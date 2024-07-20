#ifndef _ROC_ICOMPONENT_ARRAY_H_
#define _ROC_ICOMPONENT_ARRAY_H_

#include <array>
#include <unordered_map>

#include <spdlog/spdlog.h>

#include "Entity.hpp"
#include "Component.hpp"

class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray
{
public:
	bool InsertData(Entity entity, T component)
	{
		if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end())
        {
            SPDLOG_ERROR("Attempting to add another duplicate component to entity.");
            return false;
        }

		// Put new entry at end and update the maps
		size_t newIndex = mSize;
		mEntityToIndexMap[entity] = newIndex;
		mIndexToEntityMap[newIndex] = entity;
		mComponentArray[newIndex] = component;
		++mSize;
        return true;
	}

	bool RemoveData(Entity entity)
	{
		if (mEntityToIndexMap.find(entity) == mEntityToIndexMap.end())
        {
            SPDLOG_ERROR("Could not find the entity in the index map.");
            return false;
        }

		// Copy element at end into deleted element's place to maintain density
		size_t indexOfRemovedEntity = mEntityToIndexMap[entity];
		size_t indexOfLastElement = mSize - 1;
		mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

		// Update map to point to moved spot
		Entity entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
		mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
		mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

		mEntityToIndexMap.erase(entity);
		mIndexToEntityMap.erase(indexOfLastElement);

		--mSize;
        return true;
	}

	T& GetData(Entity entity)
	{
		if (mEntityToIndexMap.find(entity) == mEntityToIndexMap.end())
        {
            /** @todo PLEASE FIX THIS, I HATE THIS CODE */
			SPDLOG_ERROR("Cannot find entity in index map.");
			throw std::runtime_error("Attempted to get data from an entity that does not exist.");
        }

		// Return a reference to the entity's component
		return mComponentArray[mEntityToIndexMap[entity]];
	}

	void EntityDestroyed(Entity entity) override
	{
		if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end())
		{
			// Remove the entity's component if it existed
			RemoveData(entity);
		}
	}

private:
	// The packed array of components (of generic type T),
	// set to a specified maximum amount, matching the maximum number
	// of entities allowed to exist simultaneously, so that each entity
	// has a unique spot.
	std::array<T, MAX_ENTITIES> mComponentArray;

	// Map from an entity ID to an array index.
	std::unordered_map<Entity, size_t> mEntityToIndexMap;

	// Map from an array index to an entity ID.
	std::unordered_map<size_t, Entity> mIndexToEntityMap;

	// Total size of valid entries in the array.
	size_t mSize;
};

#endif
