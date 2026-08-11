#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "catch2/matchers/catch_matchers.hpp"
#include "ob/vwap.hpp"

#include <cmath>

using Catch::Matchers::WithinAbs;


TEST_CASE("RollingVwap full window", "[vwap]") {
    ob::RollingVwap v(3);
    v.push(100.0, 2.0);
    v.push(101.0, 3.0);
    v.push(102.0, 5.0);

    REQUIRE(v.ready());
    REQUIRE_THAT(v.value(), WithinAbs(101.3, 1e-9));
}

TEST_CASE("RollingVwap rejects zero window", "[vwap]") {
    REQUIRE_THROWS_AS(ob::RollingVwap(0), std::invalid_argument);
}

TEST_CASE("Window eviction", "[vwap]") {
    ob::RollingVwap v(3);
    v.push(100.0, 2.0);
    v.push(101.0, 3.0);
    v.push(102.0, 5.0);
    v.push(103.0, 2.0);

    REQUIRE(v.ready());
    REQUIRE_THAT(v.value(), WithinAbs(101.9, 1e-9));
}

TEST_CASE("Partial window", "[vwap]") {
    ob::RollingVwap v(3);
    v.push(100.0, 2.0);
    v.push(101.0, 3.0);

    REQUIRE_FALSE(v.ready());
    REQUIRE(std::isnan(v.value())); 
}
