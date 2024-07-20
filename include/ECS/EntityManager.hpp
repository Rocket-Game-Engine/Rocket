#pragma once

#include <queue>
#include <array>
#include <map>
#include <string>

#include <spdlog/spdlog.h>

#include "Entity.hpp"
#include "Component.hpp"

class EntityManager
{
    friend class Coordinator;

private:
    std::queue<Entity> mAvailableEntities;
    std::uint32_t mLivingCount = 0;
    std::array<Signature, MAX_ENTITIES> mSignatures;
    std::map<std::string, Entity> mEntities;

public:
    EntityManager()
    {
        for (Entity e = 0; e < MAX_ENTITIES; e++)
        {
            mAvailableEntities.push(e);
        }
    }

    /**
     * Creates an Entity with a unique ID. If there are
     * too many entities in existence, returns MAX_ENTITIES.
     * 
     * @returns On Error - MAX_ENTITIES, else the new Entity ID.
    */
    Entity CreateEntity(const std::string& ent_name)
    {
        if (mLivingCount >= MAX_ENTITIES)
        {
	        SPDLOG_ERROR("Tried to instantiate an entity past the Entity limit.");
            return MAX_ENTITIES;
        }
        Entity id = mAvailableEntities.front();
        mAvailableEntities.pop();

        mEntities.emplace(ent_name, id);

        mLivingCount++;

        return id;
    }

    Entity GetEntity(const std::string& ent_name)
    {
        std::map<std::string, Entity>::iterator it;
        if ((it = mEntities.find(ent_name)) == mEntities.end())
        {
            SPDLOG_ERROR("Tried to find entity of name {} but couldn't", ent_name);
            return MAX_ENTITIES;
        }
        return it->second;
    }

    /**
     * Takes an entity's ID and destroys the signatures
     * associated with it, as well as adding the Entity's
     * ID back to the ID queue.
     * 
     * @returns false if entity is out of range, true otherwise.
    */
    bool DestroyEntity(const std::string& ent_name)
    {
        std::map<std::string, Entity>::iterator it;
        if ((it = mEntities.find(ent_name)) == mEntities.end())
        {
            SPDLOG_ERROR("Attempted to delete an entity that does not exist! (name = {})", ent_name);
            return false;
        }

        mSignatures[it->second].reset();
        mAvailableEntities.push(it->second);
        mEntities.erase(it);
        mLivingCount--;
        return true;
    }

    void DestroyAllEntities()
    {
        if (mEntities.empty())
            return;

        std::map<std::string, Entity>::iterator before = mEntities.begin();
        std::map<std::string, Entity>::iterator after = mEntities.begin();
        after++;

        while (before != mEntities.end())
        {
            mSignatures[before->second].reset();
            mAvailableEntities.push(before->second);
            mEntities.erase(before);
            before = after;
            if (after != mEntities.end())
                after++;
        }
    }

    /**
     * Sets a particular Entity's Signature
     * 
     * @returns false if entity is out of range, true otherwise
    */
    bool SetSignature(Entity entity, Signature signature)
    {
        if (entity >= MAX_ENTITIES)
        {
	        SPDLOG_ERROR("Entity ID supplied to SetSignature is out of range.");
            return false;
        }

        mSignatures[entity] = signature;
        return true;
    }

    /**
     * Sets a particular Entity's Signature
     * 
     * @returns Signature(0) if entity is out of range, the Signature otherwise
    */
    Signature GetSignature(Entity entity)
    {
        if (entity >= MAX_ENTITIES)
        {
	        SPDLOG_ERROR("Entity ID supplied to GetSignature is out of range.");
            return Signature(0);
        }

        return mSignatures[entity];
    }
};

