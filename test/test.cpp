#include <catch2/catch_amalgamated.hpp>

#include "index_vector.hpp"

TEST_CASE("Size", "[Size]")
{
    civ::Vector<int32_t> v;
    v.push_back(0);
    v.push_back(1);
    v.push_back(2);
    REQUIRE(v.size() == 3);

    v.erase(1);
    REQUIRE(v.size() == 2);
}

TEST_CASE("ID consistency", "[ID]")
{
    civ::Vector<int32_t> v;
    for (int32_t i{0}; i < 10; ++i) {
        v.push_back(i);
    }
    REQUIRE(v.size() == 10);
    for (int32_t i{0}; i < 10; ++i) {
        REQUIRE(v[i] == i);
    }

    v.erase(1);
    REQUIRE(v[9] == 9);

    v.push_back(10);
    v.push_back(11);
    v.push_back(12);
    for (int32_t i{2}; i < 10; ++i) {
        REQUIRE(v[i] == i);
    }
}
