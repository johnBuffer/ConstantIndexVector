#include "index_vector.hpp"
#include <iostream>

struct TestStruct
{
    TestStruct() = default;
    TestStruct(int32_t i)
        : data{i}
    {}

    ~TestStruct()
    {
        std::cout << "Destroyed " << data << std::endl;
    }

    int32_t data = 0;
};

int32_t main()
{
    civ::IndexVector<TestStruct> v;
    const auto id = v.emplace_back(3);
    std::cout << id << std::endl;

    return 0;
}
