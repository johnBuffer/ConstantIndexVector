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
    //v.emplace_back(5);
    std::cout << id << std::endl;
    //v.erase(id);

    std::cout << "END" << std::endl;

    return 0;
}
