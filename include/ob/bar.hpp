#pragma once
#include <cstdint>

namespace ob {

struct Bar {
    std::int64_t ts_ns{};   // event timestamp, nanoseconds since epoch
    double open{};
    double high{};
    double low{};
    double close{};
    double volume{};
};

}  // namespace ob
