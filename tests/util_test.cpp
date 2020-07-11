#include "util/concat.h"

#include "catch2/catch.hpp"

TEST_CASE("Test concatenation", "[util]") {
    REQUIRE("3.1" == concat(3.1));
    REQUIRE("3.14" == concat(3.1,4));
    REQUIRE("3.14 = " == concat(3.1,4," =", " "));
    REQUIRE("3.14 = pi" == concat(3.1,4," =", " ", "pi"));
}