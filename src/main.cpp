#include "index_vector.hpp"
#include <iostream>

struct TestStruct
{
    explicit
    TestStruct(int32_t i)
        : data{i}
    {}

    ~TestStruct() = default;

    int32_t data = 0;
};

int32_t main()
{
    civ::IndexVector<TestStruct> v;
    v.emplace_back(3);
    v.emplace_back(5);
    v.emplace_back(7);
    v.emplace_back(9);
    v.emplace_back(11);
    v.emplace_back(12);

    v.remove_if([](const TestStruct& s) { return s.data % 3 == 0;});
    for (auto& s : v) {
        std::cout << s.data << std::endl;
    }
    std::cout << v.size() << std::endl;

    std::cout << "END" << std::endl;

    return 0;
}
