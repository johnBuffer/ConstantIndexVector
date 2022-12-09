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

    void push_back(const TObjectType& object)
    {
        const ID new_id = getSlot();
        indexes[new_id] = data.size();
        data.push_back(object);
    }

    void erase(ID id)
    {
        const ID data_id      = indexes[id];
        const ID last_data_id = data.size() - 1;
        const ID last_id      = metadata[last_data_id].rid;

        std::swap(data[data_id], data[last_data_id]);
        std::swap(metadata[data_id], metadata[last_data_id]);
        std::swap(indexes[id], indexes[last_id]);

        data.pop_back();
    }

    ID getSlot()
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
