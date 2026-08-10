// Thin driver: read bars, compute features, write CSV for the Polars diff.
// Rung 1 day 5 turns this into the cross-validation harness.

#include <cstdio>
#include <exception>
#include <string>
#include <cstddef>

#include "ob/csv.hpp"
#include "ob/extremes.hpp"
#include "ob/vwap.hpp"

int main(int argc, char** argv) {
    if (argc < 3) {
        std::fprintf(stderr, "usage: ob_features <in.csv> <out.csv> [window]\n");
        return 2;
    }

    const std::size_t window = (argc > 3) ? std::stoul(argv[3]) : 20;

    try {
        const auto bars = ob::readBars(argv[1]);

        [[maybe_unused]] ob::RollingVwap vwap(window);
        [[maybe_unused]] ob::RunningExtremes extremes;

        // TODO(rung-1 day 5): iterate bars, push into both, write a CSV with
        // columns: ts_ns,vwap,running_high,running_low,dist_from_high
        // Then tools/diff_against_polars.py compares it to the Polars output.
        std::fprintf(stderr, "read %zu bars, feature loop not written yet\n",
                     bars.size());
        std::fprintf(stderr, "output would go to %s\n", argv[2]);
        return 1;
    } catch (const std::exception& e) {
        std::fprintf(stderr, "error: %s\n", e.what());
        return 1;
    }
}
