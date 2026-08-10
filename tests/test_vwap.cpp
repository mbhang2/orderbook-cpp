#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "ob/vwap.hpp"

using Catch::Matchers::WithinAbs;

// ---------------------------------------------------------------------------
// ONE worked example, so you have the pattern. The hand computation:
//
//   window = 3
//   (price, volume) = (100.0, 2.0), (101.0, 3.0), (102.0, 5.0)
//   numerator   = 100*2 + 101*3 + 102*5 = 200 + 303 + 510 = 1013
//   denominator = 2 + 3 + 5 = 10
//   vwap        = 101.3
// ---------------------------------------------------------------------------
TEST_CASE("RollingVwap full window", "[vwap]") {
    ob::RollingVwap v(3);
    v.push(100.0, 2.0);
    v.push(101.0, 3.0);
    v.push(102.0, 5.0);

    REQUIRE(v.ready());
    REQUIRE_THAT(v.value(), WithinAbs(101.3, 1e-9));
}

// ---------------------------------------------------------------------------
// TODO(you): two more cases, computed BY HAND before you write the code.
//
//   1. Window eviction -- push a 4th observation and verify the 1st drops out.
//   2. Partial window -- what does value() return before the window fills?
//      There is no single right answer. Pick one, justify it in the README,
//      and pin it with this test.
//
// Writing the expected number first is the entire point. If you implement
// first and then read off the answer, the test proves nothing.
// ---------------------------------------------------------------------------

TEST_CASE("RollingVwap rejects zero window", "[vwap]") {
    REQUIRE_THROWS_AS(ob::RollingVwap(0), std::invalid_argument);
}
