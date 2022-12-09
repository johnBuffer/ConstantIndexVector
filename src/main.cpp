#include "index_vector.hpp"
#include <iostream>

struct Base
{
    int32_t base_attribute = 2;
};

struct Interface
{
    virtual void firstFunction() = 0;
};

struct Derived final : public Base, public Interface
{
    float a_float = 3.0f;

    ~Derived()
    {
        std::cout << "Destructor" << std::endl;
    }

    void firstFunction() override
    {
        std::cout << "This is from derived" << std::endl;
    }
};

// TODO: Use pop_back() and vector API
int32_t main()
{
    civ::Vector<Derived> v;

    std::cout << "Create" << std::endl;
    for (uint32_t i{32}; i--;) {
        v.emplace_back();
    }

    std::cout << "Remove" << std::endl;

    v.erase(15);

    std::cout << "Done" << std::endl;

    return 0;
}
