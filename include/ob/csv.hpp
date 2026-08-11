#pragma once
#include <filesystem>
#include <vector>

#include "ob/bar.hpp"

namespace ob {

struct ReadStats {
    int64_t total_rows_count{};
    int64_t failed_rows_count{};
    std::vector<int64_t> failed_rows{};
};

struct ReadResult{
    std::vector<Bar> bars{};
    ReadStats stats{};
};

// Read a CSV of bars into memory.
// Expected header: ts_ns,open,high,low,close,volume
//
// Use std::from_chars, not std::stod. Know why before you write it.
ReadResult readBars(const std::filesystem::path& path);

}  // namespace ob
