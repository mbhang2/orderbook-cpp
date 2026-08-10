#pragma once

namespace ob {

// Running session extremes and distance-from-extreme.
// Mirrors the running-extreme features in day_structure.py.
class RunningExtremes {
public:
    void push(double high, double low);
    void reset();

    [[nodiscard]] double runningHigh() const;
    [[nodiscard]] double runningLow() const;

    // Distance from the running extreme as a fraction of the running range.
    // Decide and document the behaviour when the range is zero.
    [[nodiscard]] double distanceFromHigh(double price) const;
    [[nodiscard]] double distanceFromLow(double price) const;

private:
    // TODO(rung-1 day 4): incremental tracking, O(1) per push.
    // Do NOT reach for std::max_element over history -- know why.
};

}  // namespace ob
