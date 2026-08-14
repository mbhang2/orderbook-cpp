# Rung 1 — VWAP port

## What I built
Three files were built:
- csv.cpp
    - Reads a CSV file and returns vector<Bar>
- vwap.cpp
    - RollingVwap: computes vwap for the fixed window through utilizing ring buffer.
    - SessionVwap: General vwap using cumulative volume throughout the session.
- extremes.cpp
    - Incremental session high/low
    - Relative location compared to session high/low
- Cross-validation harness against a Polars reference.

## Design decisions
See [README](../README.md#design-decisions).
## What broke
- Output path collision
    - C++ and Polars output that had to differ shared a filename
    - Output comparator returned exact zeros since it was comparing to itself
- Modifying a copy and expecting the original to change
    - `ParseDoubleAndSanityCheck` took `ptr` by value, so all five fields parsed the same position — and the OHLC sanity check passed because equal values satisfy every inequality
    - `[stat]` captured by value, so parse failures vanished from the stats

## Validation
| Statistic      | Difference |
|----------------|------------|
| Session VWAP   | 7.276e-12  |
| Rolling VWAP   | 4.366e-11  |
| Running High   | 0.00       |
| Running Low    | 0.00       |
| Dist From High | 0.00       |

Session VWAP, rolling VWAP, running high, running low, and distance from high has been computed for both C++ code and Polar code, then the values have been compared.

High, low and distance from high columns were exact since they only compare and copy.

On the other hand, vwaps have some none-zero differences because they accumulate, and floating-point addition isn't associative; C++ maintains a running sum, Polars sums fresh. Rolling VWAP is 6x worse since session VWAP only adds while rolling VWAP adds and subtracts, in which the errors accumulate much faster. 

## What I'd do differently
Parsing tatistics struct currently records the failed line numbers but not reasons. Knowing why it failed without re-reading the file would be helpful at scale.