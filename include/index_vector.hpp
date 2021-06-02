#pragma once
#include <vector>
#include <iostream>

namespace civ
{

template<typename T>
struct Ref;


struct Slot
{
	uint64_t id;
	uint64_t data_id;
};


template<typename T>
struct ObjectSlot
{
	ObjectSlot(uint64_t id_, T* object_)
		: id(id_)
		, object(object_)
	{}

	uint64_t id;
	T* object;
};


template<typename T>
struct ObjectSlotConst
{
	ObjectSlotConst(uint64_t id_, const T* object_)
		: id(id_)
		, object(object_)
	{}

	uint64_t id;
	const T* object;
};


template<typename T>
struct IndexVector
{
	IndexVector()
		: data_size(0)
		, op_count(0)
	{}
	// Data ADD / REMOVE
	template<typename... Args> 
	uint64_t emplace_back(Args&&... args);
	uint64_t push_back(const T& obj);
	void erase(uint64_t id);
	// Data access by ID
	T& operator[](uint64_t i);
	const T& operator[](uint64_t i) const;
	// Returns a standalone object allowing access to the underlying data
	Ref<T> getRef(uint64_t id);
	// Returns the data at a specific place in the data vector (not an ID)
	T& getDataAt(uint64_t i);
	// Check if the data behind the pointer is the same
	bool isValid(uint64_t i, uint64_t validity) const;
	// Returns the ID of the ith element of the data array
	uint64_t getID(uint64_t i) const;
	// Returns the ith object and id
	ObjectSlot<T> getSlotAt(uint64_t i);
	ObjectSlotConst<T> getSlotAt(uint64_t i) const;
	// Iterators
	typename std::vector<T>::iterator begin();
	typename std::vector<T>::iterator end();

	typename std::vector<T>::const_iterator begin() const;
	typename std::vector<T>::const_iterator end() const;

	// Number of objects in the array
	uint64_t size() const;

private:
	std::vector<T> data;
	std::vector<uint64_t> op_ids;
	std::vector<uint64_t> ids;
	std::vector<uint64_t> rids;
	uint64_t data_size;
	uint64_t op_count;

	bool isFull() const;
	Slot createNewSlot();
	Slot getFreeSlot();
	Slot getSlot();
	const T& getAt(uint64_t i) const;
};

template<typename T>
template<typename ...Args>
inline uint64_t IndexVector<T>::emplace_back(Args&& ...args)
{
	const Slot slot = getSlot();
	new(&data[slot.data_id]) T(args...);
	return slot.id;
}

template<typename T>
inline uint64_t IndexVector<T>::push_back(const T& obj)
{
	const Slot slot = getSlot();
	data[slot.data_id] = obj;
	return slot.id;
}

template<typename T>
inline void IndexVector<T>::erase(uint64_t id)
{
	--data_size;
	uint64_t current_data_id = ids[id];
	std::swap(data[data_size], data[current_data_id]);

	const uint64_t last_obj_id = rids[data_size];
	rids[current_data_id] = last_obj_id;
	rids[data_size] = id;
	ids[last_obj_id] = current_data_id;
	ids[id] = data_size;
}

template<typename T>
inline T& IndexVector<T>::operator[](uint64_t i)
{
	return const_cast<T&>(getAt(i));
}

template<typename T>
inline const T& IndexVector<T>::operator[](uint64_t i) const
{
	return getAt(i);
}

template<typename T>
inline ObjectSlot<T> IndexVector<T>::getSlotAt(uint64_t i)
{
	return ObjectSlot<T>(rids[i], &data[i]);
}

template<typename T>
inline ObjectSlotConst<T> IndexVector<T>::getSlotAt(uint64_t i) const
{
	return ObjectSlotConst<T>(rids[i], &data[i]);
}

template<typename T>
inline Ref<T> IndexVector<T>::getRef(uint64_t id)
{
	return Ref<T>(id, this, op_ids[id]);
}

template<typename T>
inline T& IndexVector<T>::getDataAt(uint64_t i)
{
	return data[i];
}

template<typename T>
inline uint64_t IndexVector<T>::getID(uint64_t i) const
{
	return rids[i];
}

template<typename T>
inline uint64_t IndexVector<T>::size() const
{
	return data_size;
}

template<typename T>
inline typename std::vector<T>::iterator IndexVector<T>::begin()
{
	return data.begin();
}

template<typename T>
inline typename std::vector<T>::iterator IndexVector<T>::end()
{
	return data.begin() + data_size;
}

template<typename T>
inline typename std::vector<T>::const_iterator IndexVector<T>::begin() const
{
	return data.begin();
}

template<typename T>
inline typename std::vector<T>::const_iterator IndexVector<T>::end() const
{
	return data.begin() + data_size;
}

template<typename T>
inline bool IndexVector<T>::isFull() const
{
	return data_size == data.size();
}

template<typename T>
inline Slot IndexVector<T>::createNewSlot()
{
	const uint64_t old_size = data.size();
	data.emplace_back();
	ids.push_back(old_size);
	rids.push_back(old_size);
	op_ids.push_back(op_count++);
	return { old_size, old_size };
}

template<typename T>
inline Slot IndexVector<T>::getFreeSlot()
{
	const uint64_t reuse_id = rids[data_size];
	op_ids[reuse_id] = op_count++;
	return { reuse_id, data_size };
}

template<typename T>
inline Slot IndexVector<T>::getSlot()
{
	const Slot slot = isFull() ? createNewSlot() : getFreeSlot();
	++data_size;
	return slot;
}

template<typename T>
inline const T& IndexVector<T>::getAt(uint64_t i) const
{
	return data[ids[i]];
}

template<typename T>
inline bool IndexVector<T>::isValid(uint64_t i, uint64_t validity) const
{
	return validity == op_ids[i];
}


template<typename T>
struct Ref
{
	Ref()
		: id(0)
		, array(nullptr)
		, validity_id(0)
	{}

	Ref(uint64_t id_, IndexVector<T>* a, uint64_t vid)
		: id(id_)
		, array(a)
		, validity_id(vid)
	{}

	T* operator->()
	{
		return &(*array)[id];
	}

	T& operator*()
	{
		return (*array)[id];
	}

	operator bool() const
	{
		return array->isValid(id, validity_id) && array;
	}

private:
	uint64_t id;
	IndexVector<T>* array;
	uint64_t validity_id;
};

}
