#include "ob/csv.hpp"

#include <stdexcept>

namespace ob {

std::vector<Bar> readBars(const std::filesystem::path& /*path*/) {
    // TODO(rung-1 day 2): std::ifstream, read line by line, std::from_chars.
    // Reserve the vector once you know roughly how many rows -- measure the
    // difference reserving makes, it is your first benchmark of the project.
    throw std::logic_error("readBars not implemented");
}

}  // namespace ob
