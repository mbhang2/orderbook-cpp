#include "ob/extremes.hpp"

#include <cmath>
#include <limits>

namespace ob {

void RunningExtremes::push(double high, double low) {
    highest_ = (high > highest_) ? high : highest_;
    lowest_ = (low < lowest_) ? low : lowest_;
}

void RunningExtremes::reset() {
    highest_ = -std::numeric_limits<double>::infinity();
    lowest_ = std::numeric_limits<double>::infinity();
}

double RunningExtremes::runningHigh() const {
    return highest_;
}

double RunningExtremes::runningLow() const {
    return lowest_;
}

double RunningExtremes::distanceFromHigh(double price) const {
    if (!std::isfinite(highest_ - lowest_) || highest_ == lowest_) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    return (highest_ - price) / (highest_ - lowest_);
}

double RunningExtremes::distanceFromLow(double price) const {
    if (!std::isfinite(highest_ - lowest_) || highest_ == lowest_) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    return (price - lowest_) / (highest_ - lowest_);
}

}  // namespace ob
