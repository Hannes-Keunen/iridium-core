#include <catch2/catch.hpp>

#include <iridium/core/memory.hpp>

TEST_CASE("RefCount", "[memory]")
{
    class TestStruct
    {
    public: 
        TestStruct(int value) : value(value) {}
    private:
        int value;
    };

    ir::Pool<TestStruct> pool;

    ir::Ref<TestStruct> test = pool.create(5);
    REQUIRE(test.refcount() == 1);

    SECTION("Copy semantics")
    {
        ir::Ref<TestStruct> test2 = test;
        REQUIRE(test.refcount() == 2);
    }

    SECTION("Move semantics")
    {
        ir::Ref<TestStruct> test2 = std::move(test);
        REQUIRE(test2.refcount() == 1);
        REQUIRE(test.value() == nullptr);
    }
}