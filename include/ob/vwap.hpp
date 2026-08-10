#pragma once
#include <cstddef>

namespace ob {

// Rolling volume-weighted average price over a fixed-size window.
//
// Mirrors the VWAP-relative features in day_structure.py. The C++ output must
// agree with the Polars version to 1e-9 over a full session.
class RollingVwap {
public:
    explicit RollingVwap(std::size_t window);

    // Feed one observation. The oldest drops out once the window is full.
    void push(double price, double volume);

    // Current VWAP. Behaviour before the window fills is YOUR design decision --
    // document whichever you pick in the README and make the tests match.
    [[nodiscard]] double value() const;

    [[nodiscard]] bool ready() const;
    [[nodiscard]] std::size_t window() const { return window_; }

private:
    std::size_t window_;
    // TODO(rung-1 day 3): choose your storage. Ring buffer? std::deque?
    // Whatever you pick, push() must be O(1) amortised.
};

}  // namespace ob
