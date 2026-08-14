#pragma once
#include <cstddef>
#include <vector>

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

    [[nodiscard]] double value() const;

    [[nodiscard]] bool ready() const;
    [[nodiscard]] std::size_t window() const { return window_; }

private:
    std::size_t window_;
    std::size_t head_;
    std::size_t count_;

    double cum_volume_;
    double cum_product_; 
    std::vector<std::pair<double, double>> underlying_;
};

class SessionVwap{
public:
    explicit SessionVwap();
    // Feed one observation. The oldest drops out once the window is full.
    void push(double price, double volume);

    [[nodiscard]] double value() const;

private:
    double cum_volume_;
    double cum_product_;
};

}  // namespace ob
