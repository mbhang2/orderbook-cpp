#include "ob/csv.hpp"

#include <cerrno>
#include <charconv>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <system_error>
#include <vector>


namespace ob {
std::optional<double> ParseDoubleAndSanityCheck(const char* ptr, char** end, const char* field, int row) {
    errno = 0;
    double value = std::strtod(ptr, end);
    if (*end == ptr) {
        std::cerr << "WARNING: Nothing consumed for " << field << "! (row " << row << ')' << std::endl;
        return std::nullopt;
    }
    if (errno == ERANGE) {
        std::cerr << "WARNING: Overflow for " << field << "! (row " << row << ')' << std::endl;
        return std::nullopt;
    }

    return value;
}


ReadResult readBars(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file.is_open()) {throw std::runtime_error("Error opening file!");}
    size_t estimate_size = std::filesystem::file_size(path) / 60;

    std::vector<Bar> bars;
    bars.reserve(estimate_size);
    ReadStats stat;

    std::string line;
    int64_t last_timestamp = 0;
    while (std::getline(file, line)) {
        stat.total_rows_count += 1;

        // Sanity check then skip the header
        if (stat.total_rows_count == 1) {
            if (line != "ts_ns,open,high,low,close,volume") {
                throw std::runtime_error("FATAL: Header format is different from 'ts_ns,open,high,low,close,volume'");
            }
            continue;
        }

        // Obtain the timestamp first
        int64_t timestamp;
        auto [ptr, ec] = std::from_chars(line.data(), line.data() + line.size(), timestamp);
        
        // Check for any error codes
        if (ec == std::errc::invalid_argument) {
            std::cerr << "WARNING: Invalid timestamp argument received! (row " << stat.total_rows_count << ')' << std::endl;
            stat.failed_rows.push_back(stat.total_rows_count);
            stat.failed_rows_count += 1;
            continue;
        } else if (ec == std::errc::result_out_of_range) {
            std::cerr << "WARNING: Timestamp number out of range! (row " << stat.total_rows_count << ')' << std::endl;
            stat.failed_rows.push_back(stat.total_rows_count);
            stat.failed_rows_count += 1;
            continue;
        }

        if (timestamp < last_timestamp) {
            std::cerr << "WARNING: Timestamp decreasing! (row " << stat.total_rows_count << ')' << std::endl;
            stat.failed_rows.push_back(stat.total_rows_count);
            stat.failed_rows_count += 1;
            continue;
        }
        last_timestamp = timestamp;

        // Chained parsing
        char* end{};
        auto record_failure = [&stat](std::optional<double> value) {
            if (!value) {
                stat.failed_rows_count += 1;
                stat.failed_rows.push_back(stat.total_rows_count);

                return true;
            }
            return false;
        };

        auto opt_open = ParseDoubleAndSanityCheck(ptr + 1, &end, "OPEN", stat.total_rows_count);
        if (record_failure(opt_open)) {continue;}
        ptr = end;
        double open = *opt_open;

        auto opt_high = ParseDoubleAndSanityCheck(ptr + 1, &end, "HIGH", stat.total_rows_count);
        if (record_failure(opt_high)) {continue;}
        ptr = end;
        double high = *opt_high;

        auto opt_low = ParseDoubleAndSanityCheck(ptr + 1, &end, "LOW", stat.total_rows_count);
        if (record_failure(opt_low)) {continue;}
        ptr = end;
        double low = *opt_low;

        auto opt_close = ParseDoubleAndSanityCheck(ptr + 1, &end, "CLOSE", stat.total_rows_count);
        if (record_failure(opt_close)) {continue;}
        ptr = end;
        double close = *opt_close;

        auto opt_volume = ParseDoubleAndSanityCheck(ptr + 1, &end, "VOLUME", stat.total_rows_count);
        if (record_failure(opt_volume)) {continue;}
        ptr = end;
        double volume = *opt_volume;
        
        // Sanity check OHLC
        if (high < low || high < open || high < close || low > open || low > close || volume < 0) {
            std::cerr << "WARNING: OHLCV sanity check failed! (row " << stat.total_rows_count << ')' << std::endl;
            stat.failed_rows_count += 1;
            stat.failed_rows.push_back(stat.total_rows_count);
            continue;
        }

        bars.push_back(Bar(timestamp, open, high, low, close, volume));
    }

    return ReadResult(std::move(bars), stat);
}

}  // namespace ob
