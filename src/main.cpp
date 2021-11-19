#include <iostream>
#include <unordered_map>
#include <vector>
#include <chrono>
#include "number_generator.hpp"


std::string createRandomName(int length)
{
    std::string res;

    for (uint32_t i(length); i--;) {
        res += 'a' + static_cast<char>(RNGi32::getUnder(26));
    }

    return res;
}


struct Entity
{
    std::string name;
    Entity*     parent;
    uint32_t    depth;
    bool        status;

    Entity() = default;
    Entity(const std::string& n)
        : name(n)
    {}
};

// Get time stamp in microseconds.
uint64_t micros()
{
    uint64_t us = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::
                                                                        now().time_since_epoch()).count();
    return us;
}


const Entity& findInMap(const std::string& s, std::unordered_map<std::string, Entity>& m)
{
    return m[s];
}

const Entity& findInVec(const std::string& s, std::vector<Entity>& v)
{
    for (const Entity& e : v) {
        if (e.name == s) {
            return e;
        }
    }
}


int main()
{
    std::vector<std::string> names;
    std::unordered_map<std::string, Entity> entity_map;
    std::vector<Entity> entity_vector;

    uint32_t e_map_count = 0;
    uint32_t e_vec_count = 0;

    const uint32_t name_length = 320;
    //const uint32_t count = 35;
    for (uint32_t count = 1; count<101; ++count) {
        const uint32_t run_count = 20000;
        double mean_map = 0.0;
        double mean_vec = 0.0;

        names.clear();
        for (uint32_t i(count); i--;) {
            names.push_back(createRandomName(name_length));
        }
        entity_map.clear();
        entity_vector.clear();
        for (const std::string &name: names) {
            entity_map[name] = Entity{name};
            entity_vector.emplace_back(name);
        }

        for (uint32_t i(run_count); i--;) {
            const uint32_t name_index = RNGf::getUnder(names.size());
            const std::string &name = names[name_index];
            auto start = micros();
            const Entity &e_map = findInMap(name, entity_map);
            mean_map += static_cast<double>(micros() - start);
            e_map_count += e_map.depth;

            start = micros();
            const Entity &e_vec = findInVec(name, entity_vector);
            mean_vec += static_cast<double>(micros() - start);
            e_vec_count += e_vec.depth;
        }

        std::cout << count << ";" << mean_map / static_cast<double>(run_count) << ";" << mean_vec / static_cast<double>(run_count) << std::endl;
    }

    std::cout << e_map_count << std::endl;
    std::cout << e_vec_count << std::endl;

    return 0;
}
