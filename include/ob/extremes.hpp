#pragma once

#include <limits>
namespace ob {

// Running session extremes and distance-from-extreme.
// Mirrors the running-extreme features in day_structure.py.
class RunningExtremes {
public:
    void push(double high, double low);
    void reset();

    [[nodiscard]] double runningHigh() const;
    [[nodiscard]] double runningLow() const;

    [[nodiscard]] double distanceFromHigh(double price) const;
    [[nodiscard]] double distanceFromLow(double price) const;

private:
    // TODO(rung-1 day 4): incremental tracking, O(1) per push.
    // Do NOT reach for std::max_element over history -- know why.
    double highest_ = -std::numeric_limits<double>::infinity();
    double lowest_ = std::numeric_limits<double>::infinity();
};

}  // namespace ob
