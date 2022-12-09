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
class IndexVector;

template<typename TObjectType>
class Ref
{
public:
    Ref() = default;
    Ref(ID id, ID validity_id, IndexVector<TObjectType>* vector)
        : m_id{id}
        , m_validity_id{validity_id}
        , m_vector{vector}
    {}

    TObjectType* operator->()
    {
        return &(*m_vector)[m_id];
    }

    const TObjectType* operator->() const
    {
        return &(*m_vector)[m_id];
    }

    TObjectType& operator*()
    {
        return (*m_vector)[m_id];
    }

    const TObjectType& operator*() const
    {
        return (*m_vector)[m_id];
    }

    [[nodiscard]]
    ID getID() const
    {
        return m_id;
    }

    explicit
    operator bool() const
    {
        return m_vector && m_vector->isValid(m_id, m_validity_id);
    }

private:
    ID                        m_id          = 0;
    ID                        m_validity_id = 0;
    IndexVector<TObjectType>* m_vector      = nullptr;
};

template<typename TObjectType>
class IndexVector
{
public:
    IndexVector() = default;

    ID push_back(const TObjectType& object)
    {
        const ID id = getSlot();
        data.push_back(object);
        return id;
    }

    template<typename... TArgs>
    ID emplace_back(TArgs&&... args)
    {
        const ID id = getSlot();
        data.emplace_back(std::forward<TArgs>(args)...);
        return id;
    }

    void erase(ID id)
    {
        // Fetch relevant info
        const ID data_id      = indexes[id];
        const ID last_data_id = data.size() - 1;
        const ID last_id      = metadata[last_data_id].rid;
        // Invalidate metadata
        metadata[data_id].validity_id = operation_count++;
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

    Ref<TObjectType> getRef(ID id)
    {
        return {id, metadata[indexes[id]].validity_id, this};
    }

    [[nodiscard]]
    bool isValid(ID id, ID validity_id) const
    {
        return validity_id == metadata[indexes[id]].validity_id;
    }

    typename std::vector<TObjectType>::iterator begin() noexcept
    {
        return data.begin();
    }

    typename std::vector<TObjectType>::iterator end() noexcept
    {
        return data.end();
    }

    typename std::vector<TObjectType>::const_iterator begin() const noexcept
    {
        return data.begin();
    }

    typename std::vector<TObjectType>::const_iterator end() const noexcept
    {
        return data.end();
    }

private:
    std::vector<TObjectType> data;
    std::vector<Metadata>    metadata;
    std::vector<ID>          indexes;

    uint64_t operation_count = 0;
};

}
