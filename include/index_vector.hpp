#pragma once
#include <vector>


namespace civ
{

using ID = uint64_t;

struct Metadata
{
    ID validity_id = 0;
    ID rid         = 0;
};

template<typename TObjectType>
class IndexVector
{
public:
    IndexVector() = default;

    ID push_back(const TObjectType& object)
    {
        data.push_back(object);
        return getSlot();
    }

    template<typename... TArgs>
    ID emplace_back(const TArgs&&... args)
    {
        data.emplace_back(std::forward<TArgs>(args)...);
        return getSlot();
    }

    void erase(ID id)
    {
        // Fetch relevant info
        const ID data_id      = indexes[id];
        const ID last_data_id = data.size() - 1;
        const ID last_id      = metadata[last_data_id].rid;
        // Swap the object to delete with the object at the end
        std::swap(data[data_id], data[last_data_id]);
        std::swap(metadata[data_id], metadata[last_data_id]);
        std::swap(indexes[id], indexes[last_id]);
        // Destroy the object
        data.pop_back();
    }

    ID getSlot()
    {
        const ID id = getSlotID();
        indexes[id] = data.size();
        return id;
    }

    ID getSlotID()
    {
        // This means that we have available slots
        if (metadata.size() > data.size()) {
            metadata[data.size()].validity_id = operation_count++;
            return metadata[data.size()].rid;
        }
        // A new slot has to be created
        const ID new_id = data.size();
        metadata.push_back({new_id, operation_count++});
        indexes.push_back(new_id);
        return new_id;
    }

    TObjectType& operator[](ID id)
    {
        return data[indexes[id]];
    }

    [[nodiscard]]
    size_t size() const
    {
        return data.size();
    }

private:
    std::vector<TObjectType> data;
    std::vector<Metadata>    metadata;
    std::vector<ID>          indexes;

    uint64_t operation_count = 0;
};

}
