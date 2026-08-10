#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

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
