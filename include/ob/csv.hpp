#pragma once
#include <filesystem>
#include <vector>

#include "ob/bar.hpp"

namespace ob {

// Read a CSV of bars into memory.
// Expected header: ts_ns,open,high,low,close,volume
//
// Use std::from_chars, not std::stod. Know why before you write it.
std::vector<Bar> readBars(const std::filesystem::path& path);

}  // namespace ob
