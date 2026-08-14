// Thin driver: read bars, compute features, write CSV for the Polars diff.
// Rung 1 day 5 turns this into the cross-validation harness.

#include <cstdio>
#include <exception>
#include <fstream>
#include <iomanip>
#include <stdexcept>
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
        const auto [bars, result] = ob::readBars(argv[1]);

        ob::SessionVwap svwap;
        ob::RollingVwap rvwap(window);
        ob::RunningExtremes extremes;

        // TODO(rung-1 day 5): iterate bars, push into both, write a CSV with
        // columns: ts_ns,vwap,running_high,running_low,dist_from_high
        // Then tools/diff_against_polars.py compares it to the Polars output.
        std::ofstream file(argv[2]);

        if (!file.is_open()) {
            throw std::runtime_error("FATAL: Cannot open file!");
        }

        file << "ts_ns,session_vwap,rolling_vwap,running_high,running_low,dist_from_high" << "\n"; 

        for (auto [ts_ns, open, high, low, close, volume] : bars) {
            svwap.push((high + low + close)/3, volume);
            rvwap.push((high + low + close)/3, volume);
            extremes.push(high, low);
            
            file << std::setprecision(17) << ts_ns << "," << svwap.value() 
                    << "," << rvwap.value() << "," << extremes.runningHigh() << "," << extremes.runningLow() << "," << extremes.distanceFromHigh(close) << "\n";
        }
        file.close();

        return 0;
    } catch (const std::exception& e) {
        std::fprintf(stderr, "error: %s\n", e.what());
        return 1;
    }
}
