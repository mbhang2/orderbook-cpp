#!/usr/bin/env python3
"""Cross-validation harness -- Rung 1, Day 5.

Runs the Polars feature computation and the C++ binary over the same input,
then diffs column by column.

Fill in polars_features(). It should call into the SAME code path that
day_structure.py uses, not a reimplementation -- otherwise you are comparing
two new things instead of validating one against a known-good one.
"""

from __future__ import annotations

import argparse
import subprocess
import sys
from pathlib import Path

import polars as pl

TOLERANCE = 1e-9


def polars_features(bars_csv: Path, window: int) -> pl.DataFrame:
    df = pl.read_csv(bars_csv)

    tp = (pl.col("high") + pl.col("low") + pl.col("close")) / 3
    pv = tp * pl.col("volume")

    df = df.with_columns([
        pv.alias("_pv"),
        pl.col("volume").alias("_v"),
    ])

    df = df.with_columns([
        (pl.col("_pv").cum_sum() / pl.col("_v").cum_sum()).alias("session_vwap"),
        (
            pl.col("_pv").rolling_sum(window_size=window)
            / pl.col("_v").rolling_sum(window_size=window)
        ).alias("rolling_vwap"),
        pl.col("high").cum_max().alias("running_high"),
        pl.col("low").cum_min().alias("running_low"),
    ])

    rng = pl.col("running_high") - pl.col("running_low")
    df = df.with_columns([
        pl.when(rng == 0)
        .then(None)
        .otherwise((pl.col("running_high") - pl.col("close")) / rng)
        .alias("dist_from_high"),
    ])

    # C++ writes NaN where a value is undefined; Polars produces null.
    # Convert so both sides speak the same language.
    out = df.select([
        "ts_ns", "session_vwap", "rolling_vwap",
        "running_high", "running_low", "dist_from_high",
    ])
    return out.with_columns([
        pl.col(c).fill_null(float("nan"))
        for c in ("session_vwap", "rolling_vwap", "dist_from_high")
    ])


def cpp_features(binary: Path, bars_csv: Path, out_csv: Path, window: int) -> pl.DataFrame:
    subprocess.run(
        [str(binary), str(bars_csv), str(out_csv), str(window)],
        check=True,
    )
    return pl.read_csv(out_csv, schema_overrides={"rolling_vwap": pl.Float64})


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("bars_csv", type=Path)
    ap.add_argument("--binary", type=Path, default=Path("build/ob_features"))
    ap.add_argument("--out", type=Path, default=Path("data/cpp_out.csv"))
    ap.add_argument("--window", type=int, default=20)
    args = ap.parse_args()

    want = polars_features(args.bars_csv, args.window)
    got = cpp_features(args.binary, args.bars_csv, args.out, args.window)

    if want.height != got.height:
        print(f"row count mismatch: polars={want.height} cpp={got.height}")
        return 1

    worst = 0.0
    failed = False
    for col in ("session_vwap", "rolling_vwap", "running_high", "running_low", "dist_from_high"):
        diff = (want[col] - got[col]).abs().max()
        worst = max(worst, float(diff))
        status = "ok" if diff <= TOLERANCE else "FAIL"
        print(f"{col:<16} max_abs_diff={diff:.3e}  {status}")
        failed |= diff > TOLERANCE

    print(f"\nworst overall: {worst:.3e} (tolerance {TOLERANCE:.0e})")
    if failed:
        print("\nMismatch. Before reaching for a debugger: is this accumulation")
        print("order? Look up IEEE 754 non-associativity and Kahan summation.")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
