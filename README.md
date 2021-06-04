# ConstantIndexVector

Simple `constant indexed vector` header only library, requires C++11.

## Features
 - An `index` will always reference to the same object even after deletions or additions, as long as the object hasn't been erased (IDs are reused).
 - Contiguous data in memory (no gap between objects)
 - **O(1)** deletions
 - **O(1)** additions
 - **O(1)** access

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
civ::IndexVector<int32_t> v;

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

## Overhead

 - **24 additional bytes** per object
 - **Access** requires 1 indirection
 - **Deletion** requires 1 object swap
 - **No overhead for iteration**
