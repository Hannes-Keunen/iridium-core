#include <catch2/catch.hpp>

#include <iridium/core/string.hpp>

TEST_CASE("Strings", "[string]")
{
    ir::String str = "Hello, world!";
    REQUIRE(str.size() == std::strlen("Hello, world!"));

    SECTION("comparison with char*")
    {
        REQUIRE(str == "Hello, world!");
        REQUIRE("Hello, world!" == str);
        REQUIRE(str != "Goodbye, world");
        REQUIRE("Goodbye, world" != str);
        REQUIRE(str > "Aello, world!");
        REQUIRE("Aello, world" < str);
        REQUIRE(str < "Hello, worlz!");
        REQUIRE("Hello, worlz!" > str);
    }

    SECTION("comparison with String")
    {
        REQUIRE(str == ir::String("Hello, world!"));
        REQUIRE(str != ir::String("Goodbye, world"));
        REQUIRE(str > ir::String("Aello, world!"));
        REQUIRE(str < ir::String("Hello, worlz!"));
    }
}
