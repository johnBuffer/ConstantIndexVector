#include "index_vector.hpp"
#include <iostream>


int32_t main()
{
    civ::IndexVector<int32_t> v;
    
    const uint64_t id_1 = v.push_back(2);
    const uint64_t id_2 = v.push_back(3);
    const uint64_t id_3 = v.push_back(5);
    const uint64_t id_4 = v.push_back(7);

    v.erase(id_1);
    v.erase(id_3);

    std::cout << v[id_2] << std::endl;
    std::cout << v[id_4] << std::endl;

    civ::Ref<int32_t> ref = v.getRef(id_2);
    std::cout << *ref << std::endl;
    
    for (int32_t i : v) {
        std::cout << i << std::endl;
    }

    return 0;
}
