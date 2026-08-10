#include "ob/vwap.hpp"

#include <stdexcept>

namespace ob {

RollingVwap::RollingVwap(std::size_t window) : window_(window) {
    if (window == 0) {
        throw std::invalid_argument("window must be > 0");
    }
    // TODO(rung-1 day 3): reserve your storage here.
}

void RollingVwap::push(double /*price*/, double /*volume*/) {
    throw std::logic_error("RollingVwap::push not implemented");
}

double RollingVwap::value() const {
    throw std::logic_error("RollingVwap::value not implemented");
}

bool RollingVwap::ready() const {
    throw std::logic_error("RollingVwap::ready not implemented");
}

}  // namespace ob
