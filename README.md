# ConstantIndexVector

Simple `constant indexed vector` header only library, requires C++11.

## Features
 - An `index` will always reference to the same object even after deletions or additions, as long as the object hasn't been erased (IDs are reused).
 - Contiguous m_data in memory (no gap between objects)
 - **O(1)** deletions
 - **O(1)** additions
 - **O(1)** access
 - Standalone references
 - Polymorphic standalone references

## Limitations
 - **Order is not guaranteed to be maintained**
 - Objects need to support `std::move`
 - Size can only **increase** 

## Example
```c++
// Include the header
#include "index_vector.hpp"
```

```c++
// Create the vector
civ::Vector<int32_t> v;

// Add objects
const uint64_t id_1 = v.push_back(2);
const uint64_t id_2 = v.push_back(3);
const uint64_t id_3 = v.push_back(5);
const uint64_t id_4 = v.push_back(7);

// Iterate over the objects
for (int32_t i : v) {
    std::cout << i << std::endl;
}

// Mutate the vector
v.erase(id_1);
v.erase(id_3);
v.push_back(11);

// Access objects
std::cout << v[id_2] << std::endl; // 3
std::cout << v[id_4] << std::endl; // 7
```

## Standalone references

It is possible to create standalone references that can provide access to the object without providing the vector alongside.

```c++
civ::Ref<int32_t> ref = v.getRef(id_2);
std::cout << *ref << std::endl; // 3
```

Since IDs can be reused you can check the reference's validity like this:

```c++
if (ref) {
    std::cout << *ref << std::endl; // 3
} else {
    // The objects has been erased, the reference can return another value. (Won't cause a segfault if accessed)
}
```

## Polymorphic references

It is possible to create polymorphic references and convert them to a reference to a base or derived class.
```c++
// Create base class
struct Base
{
    int32_t base_attribute;
};
// Create derived class
struct Derived : public Base
{
    int32_t derived_attribute;
};
// Create a vector of derived class
civ::Vector<Derived> v;
v.emplace_back();
// Get a reference to the object
civ::PRef<Derived> ref_to_derived = v.getPRef(0);
// Convert it to a reference to a base class
civ::PRef<Base> ref_to_base = ref_to_derived;
```

## Overhead

 - **24 additional bytes** per object
 - **Access** requires 1 indirection
 - **Deletion** requires 1 object swap
 - **No overhead for iteration**
