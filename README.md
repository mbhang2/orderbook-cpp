# orderbook-cpp

Rungs 1–6 of the Track A coding ladder, in one repo. The commit history is the
deliverable as much as the code is — an interviewer scrolling from "naive VWAP
port" to "lock-free feed handler" learns more than any summary.

## Quickstart

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build
ctest --test-dir build --output-on-failure
```

First run: **the build is clean and 2 of 3 tests fail.** That is correct. The
headers declare the interface; the bodies throw `std::logic_error`. Your job is
to turn them green.

## Layout

```
include/ob/     public headers — the interface is already designed
src/            implementations — stubs for you to fill
tests/          Catch2 tests — one worked example, the rest TODO
tools/          diff_against_polars.py, the Rung 1 Day 5 harness
data/           sample_bars.csv (200 synthetic NQ-ish bars), gitignored otherwise
```

## Rules for this repo

- **Rungs 1–2: no LLM writes code here.** cppreference and official docs only.
  Stuck? Twenty minutes before you look anything up.
- Commit at the end of every session, even broken. The message says what you
  tried.
- Tag rung completions: `git tag rung-1`, etc.
- Every performance claim needs a before and after number in this README.

## Order of work — Rung 1

| Day | File | Task |
|---|---|---|
| 1 | — | Build green, `ctest` runs, one dummy test passes ✅ *(done — you're here)* |
| 2 | `src/csv.cpp` | `readBars` with `std::from_chars` |
| 3 | `src/vwap.cpp` | `RollingVwap`, plus your two hand-computed tests |
| 4 | `src/extremes.cpp` | `RunningExtremes`, plus the zero-range policy |
| 5 | `src/main.cpp`, `tools/` | Feature loop writes CSV; harness diffs it |
| 6 | — | Chase the mismatch. It's accumulation order. Kahan summation. |
| 7 | `README.md` | Write up what you built and what surprised you. Tag `rung-1`. |

## Design decisions

*Fill this in as you go. Two or three sentences each. This section is what gets
read in an interview, not the code.*

- **Storage for `RollingVwap`:** _(ring buffer vs deque — pick and justify)_
- **`value()` before the window fills:** _(your policy)_
- **Zero-range behaviour in `distanceFrom*`:** _(your policy)_

## Benchmarks

| Rung | Change | Before | After | Kept? |
|---|---|---|---|---|
| | | | | |

*Including the changes you reverted. Especially those.*
