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

    auto ref = v.getRef(0);
    std::cout << ref->data << std::endl;

    v.erase(0);
    std::cout << "Valid " << bool(ref) << std::endl;


    for (auto& o : v) {
        std::cout << o.data << std::endl;
    }

    std::cout << "END" << std::endl;

    return 0;
}
