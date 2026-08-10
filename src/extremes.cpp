#include "ob/extremes.hpp"

#include <stdexcept>

namespace ob {

void RunningExtremes::push(double /*high*/, double /*low*/) {
    throw std::logic_error("RunningExtremes::push not implemented");
}

void RunningExtremes::reset() {
    throw std::logic_error("RunningExtremes::reset not implemented");
}

double RunningExtremes::runningHigh() const {
    throw std::logic_error("RunningExtremes::runningHigh not implemented");
}

double RunningExtremes::runningLow() const {
    throw std::logic_error("RunningExtremes::runningLow not implemented");
}

double RunningExtremes::distanceFromHigh(double /*price*/) const {
    throw std::logic_error("RunningExtremes::distanceFromHigh not implemented");
}

double RunningExtremes::distanceFromLow(double /*price*/) const {
    throw std::logic_error("RunningExtremes::distanceFromLow not implemented");
}

}  // namespace ob
