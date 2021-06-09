#include "index_vector.hpp"
#include <iostream>


int32_t main()
{
    std::vector<uint64_t> ids;
    civ::Vector<int32_t> v;
    
    for (uint32_t i(0); i<10000; ++i) {
        const uint64_t id = v.push_back(rand()%10000);
        ids.push_back(id);
    }

    for (uint32_t i(0); i<1000; ++i) {
        // Choose an item to delete
        const uint64_t id_i = rand()%v.size();
        const uint64_t id = ids[id_i];
        std::swap(ids[id_i], ids.back());
        ids.pop_back();
        v.erase(id);
    }

    for (uint32_t i(0); i<1000; ++i) {
        const uint64_t id = v.push_back(rand()%10000);
        ids.push_back(id);
    }

    for (uint64_t id : v.ids) {
        if (id > 10000) {
            std::cout << id << std::endl;
        }
    }

    return 0;
}
