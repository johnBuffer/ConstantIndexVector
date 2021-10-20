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

struct Derived : public Base, public Interface
{
    float a_float = 3.0f;

    void firstFunction() override
    {
        std::cout << "This is from derived" << std::endl;
    }
};


int32_t main()
{
    civ::Vector<Derived> v;
    v.emplace_back();

    civ::PRef<Interface> 

    return 0;
}
