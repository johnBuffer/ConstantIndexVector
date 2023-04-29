#include <catch2/catch_amalgamated.hpp>
#include "index_vector.hpp"


struct TestStruct
{
    explicit
    TestStruct(int32_t i) : data{i} {}
    int32_t data = 0;
};


TEST_CASE("Capacity", "[empty]")
{
    civ::IndexVector<int32_t> v;
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 0);
}

TEST_CASE("Empty", "[reserve]")
{
    civ::IndexVector<int32_t> v;
    v.reserve(10);
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 10);
}

TEST_CASE("Index", "[index 0]")
{
    civ::IndexVector<int32_t> v;
    const auto id = v.push_back(10);
    REQUIRE(v.size() == 1);
    REQUIRE(id == 0);
    REQUIRE(v[id] == 10);
}

TEST_CASE("Index", "[index 1]")
{
    civ::IndexVector<int32_t> v;
    const auto id1 = v.push_back(10);
    const auto id2 = v.push_back(12);
    REQUIRE(v.size() == 2);
    REQUIRE(id1 == 0);
    REQUIRE(id2 == 1);
    REQUIRE(v[id1] == 10);
    REQUIRE(v[id2] == 12);
}

TEST_CASE("Index", "[index access after delete]")
{
    civ::IndexVector<int32_t> v;
    const auto id1 = v.push_back(10);
    const auto id2 = v.push_back(12);
    REQUIRE(id1 == 0);
    REQUIRE(id2 == 1);
    REQUIRE(v[id1] == 10);
    REQUIRE(v[id2] == 12);

    v.erase(0);
    REQUIRE(v.size() == 1);
    REQUIRE(v[id2] == 12);
}

TEST_CASE("Index", "[index reuse]")
{
    civ::IndexVector<int32_t> v;
    const auto id1 = v.push_back(10);
    const auto id2 = v.push_back(12);
    REQUIRE(id1 == 0);
    REQUIRE(id2 == 1);
    REQUIRE(v[id1] == 10);
    REQUIRE(v[id2] == 12);

    v.erase(0);
    REQUIRE(v.size() == 1);
    REQUIRE(v[id2] == 12);

    const auto id3 = v.push_back(14);
    REQUIRE(v.size() == 2);
    REQUIRE(id3 == id1);
    REQUIRE(v[id3] == 14);
    REQUIRE(v[id2] == 12);
}

TEST_CASE("Remove if", "[remove if pair]")
{
    civ::IndexVector<uint32_t> v;
    // Add 10 numbers, v[i] == i
    for (uint32_t i{0}; i<10; ++i) {
        v.push_back(i);
    }
    REQUIRE(v.size() == 10);

    // Remove even numbers
    v.remove_if([](uint32_t i) {return i%2 == 0;});
    REQUIRE(v.size() == 5);
    for (uint32_t i{0}; i < 10; ++i) {
        if (i%2 != 0) {
            REQUIRE(v[i] == i);
        }
    }
    // Remove odd numbers
    v.remove_if([](uint32_t i) {return i%2 == 1;});
    REQUIRE(v.size() == 0);
}

TEST_CASE("Ref", "[invalid ref]")
{
    civ::Ref<int32_t> ref;
    REQUIRE_FALSE(ref);
}

TEST_CASE("Ref", "[valid ref]")
{
    civ::IndexVector<int32_t> v;
    const auto id = v.push_back(-2);
    civ::Ref<int32_t> ref = v.createRef(id);
    REQUIRE(ref);
    REQUIRE(*ref == -2);
}

TEST_CASE("Ref", "[delete ref]")
{
    civ::IndexVector<int32_t> v;
    const auto id = v.push_back(-2);
    civ::Ref<int32_t> ref = v.createRef(id);
    v.erase(ref);
    REQUIRE(v.size() == 0);
    REQUIRE_FALSE(ref);
}

TEST_CASE("Ref", "[outdated ref]")
{
    civ::IndexVector<int32_t> v;
    const auto id = v.push_back(-2);
    civ::Ref<int32_t> ref = v.createRef(id);
    v.erase(id);
    v.push_back(-2);
    // Value at id 0 is the same
    REQUIRE(*ref == -2);
    // Ref is now invalid because the object has been erased
    REQUIRE_FALSE(ref);
}

TEST_CASE("Struct", "[struct push_back]")
{
    civ::IndexVector<TestStruct> v;
    const auto id = v.push_back(TestStruct{-2});
    REQUIRE(v[id].data == -2);
}

TEST_CASE("Struct", "[struct emplace_back]")
{
    civ::IndexVector<TestStruct> v;
    const auto id = v.emplace_back(-2);
    REQUIRE(v[id].data == -2);
}

TEST_CASE("Ref", "[struct arrow]")
{
    civ::IndexVector<TestStruct> v;
    const auto id = v.emplace_back(-2);
    auto ref = v.createRef(id);
    REQUIRE((*ref).data == -2);
    REQUIRE(ref->data == -2);
}

TEST_CASE("Capacity", "[reserve empty]")
{
    civ::IndexVector<TestStruct> v;
    v.reserve(10);
    REQUIRE(v.size() == 0);
    REQUIRE(v.empty());
}

TEST_CASE("Capacity", "[reserve push_back]")
{
    civ::IndexVector<int32_t> v;
    v.reserve(10);
    const auto id = v.push_back(1);
    REQUIRE(id == 0);
    REQUIRE(v.size() == 1);
    REQUIRE(v.capacity() == 10);
}

TEST_CASE("Capacity", "[reserve multiple push_back]")
{
    civ::IndexVector<uint32_t> v;
    v.reserve(1000);
    uint32_t* ptr_start = v.data();
    for (uint32_t i{0}; i<1000; ++i) {
        v.push_back(i);
    }
    REQUIRE(ptr_start == v.data());

    for (uint32_t i{0}; i<1000; ++i) {
        REQUIRE(v[i] == i);
    }
}
