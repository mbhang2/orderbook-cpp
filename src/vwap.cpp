#include "ob/vwap.hpp"

#include <limits>
#include <stdexcept>

namespace ob {

RollingVwap::RollingVwap(std::size_t window) : window_(window) {
    if (window == 0) {
        throw std::invalid_argument("window must be > 0");
    }

    head_ = 0;
    count_ = 0;

    cum_volume_ = 0;
    cum_product_ = 0;
    underlying_.resize(window);
}

void RollingVwap::push(double price, double volume) {
    // Retreive what's leaving and insert new
    auto [leaving_price, leaving_vol] = underlying_.at(head_);
    underlying_.at(head_) = std::pair<double, double>(price, volume);

    // Update cumulative
    cum_volume_ = cum_volume_ - leaving_vol + volume;
    cum_product_ = cum_product_ - leaving_price * leaving_vol + price * volume;

    // Move head accordingly
    head_ = (head_ + 1) % window_;
    if (count_ < window_) {count_ += 1;}
}

double RollingVwap::value() const {
    if (ready()) {
        return cum_product_ / cum_volume_;
    }
    return std::numeric_limits<double>::quiet_NaN();
}

bool RollingVwap::ready() const {
    return count_ == window_;
}

}  // namespace ob
