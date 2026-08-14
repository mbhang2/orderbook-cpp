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
| 1 | — | Build green, `ctest` runs, one dummy test passes
| 2 | `src/csv.cpp` | `readBars` with `std::from_chars` |
| 3 | `src/vwap.cpp` | `RollingVwap`, plus your two hand-computed tests |
| 4 | `src/extremes.cpp` | `RunningExtremes`, plus the zero-range policy |
| 5 | `src/main.cpp`, `tools/` | Feature loop writes CSV; harness diffs it |
| 6 | — | Chase the mismatch. It's accumulation order. Kahan summation. |
| 7 | `README.md` | Write up what you built and what surprised you. Tag `rung-1`. |

## Design decisions
- **Storage for `RollingVwap`:** 
  - rRing buffer over std::vector, sized at construction.
  - Chose it over std::deque for contiguous memory and a single allocation; deque would have been simpler to write (push_back/pop_front, no index arithmetic and no separate count). At W = 20–50 the difference is likely below measurement noise — untested. 
  - Revisit if push shows up in a Rung 4 profile.
- **`value()` before the window fills:**
  - A partial VWAP is type-identical and magnitude-plausible. After three observations of a fifty-bar window it returns something like 21003.4 — same double, same range as a real value. Nothing at the call site can tell the two apart. 
  - NaN can't be mistaken for a price, and it propagates: any arithmetic downstream stays NaN, so a consumer that ignores ready() still can't quietly produce a number that looks fine.
- **Zero-range behaviour in `distanceFrom*`:**
  - Similar to `value()`, to prevent division by zero operation, `distanceFrom*` will return NaN if the range is zero.

## Benchmarks

| Rung | Change | Before | After | Kept? |
|---|---|---|---|---|
| | | | | |

*Including the changes you reverted. Especially those.*
