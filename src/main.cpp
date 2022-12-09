#include "index_vector.hpp"
#include <iostream>

struct TestStruct
{
    explicit
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
    v.emplace_back(3);
    v.emplace_back(5);
    v.emplace_back(7);
    v.emplace_back(11);

    v.erase(0);

    for (auto& o : v) {
        std::cout << o.data << std::endl;
    }

    std::cout << "END" << std::endl;

    return 0;
}
