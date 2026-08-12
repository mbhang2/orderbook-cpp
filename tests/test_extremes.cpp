#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <cmath>
#include <limits>

#include "catch2/matchers/catch_matchers.hpp"
#include "ob/extremes.hpp"

using Catch::Matchers::WithinAbs;

TEST_CASE("RunningExtremes tracks session high and low", "[extremes]") {
    ob::RunningExtremes e;
    e.push(101.0, 99.0);
    e.push(103.0, 100.0);
    e.push(102.0, 98.0);

    REQUIRE_THAT(e.runningHigh(), WithinAbs(103.0, 1e-12));
    REQUIRE_THAT(e.runningLow(), WithinAbs(98.0, 1e-12));
}

// ---------------------------------------------------------------------------
// TODO(you):
//   1. distanceFromHigh / distanceFromLow -- hand-compute one case.
//   2. reset() clears state.
//   3. The degenerate case: high == low, so range == 0. Division by zero is
//      waiting for you here. Decide the policy, document it, test it.
// ---------------------------------------------------------------------------
TEST_CASE("RunningExtremes distanceFrom*", "[extremes]") {
    ob::RunningExtremes e;
    e.push(101.0, 99);
    e.push(103.0, 100.0);
    e.push(102.0, 98.0);

    REQUIRE_THAT(e.distanceFromHigh(100.0), WithinAbs(0.6, 1e-12));
    REQUIRE_THAT(e.distanceFromLow(99.0), WithinAbs(0.2, 1e-12));
}

TEST_CASE("RunningExtremes reset clears state", "[extremes]") {
    ob::RunningExtremes e;
    e.push(101.0, 99.0);
    e.reset();

    REQUIRE(!std::isfinite(e.runningHigh()));
    REQUIRE(!std::isfinite(e.runningLow()));
}

TEST_CASE("RunningExtremes handling zero range", "[extremes]") {
    ob::RunningExtremes e;
    e.push(100.0, 100.0);

    REQUIRE(std::isnan(e.distanceFromHigh(100.0)));
    REQUIRE(std::isnan(e.distanceFromLow(100.0)));
}