#include "index_vector.hpp"
#include <iostream>

// TODO: Use pop_back() and vector API
int32_t main()
{
    civ::IndexVector<int32_t> v;
    v.push_back(0);

    std::cout << v[0] << std::endl;
    std::cout << v.size() << std::endl;

    v.erase(0);
    std::cout << v.size() << std::endl;

    return 0;
}
