#pragma once

/**
 * @file Coordinator.hpp
 * 
 * This file contains the definition and (most of) the
 * implementation of the engine's Coordinator class.
 * 
 * @note This, and most base ECS classes, were adapted from
 * [Austin Morlan's amazing ECS writeup here.](https://austinmorlan.com/posts/entity_component_system/)
 * 
 * @author Tim Bishop
*/

#include "EntityManager.hpp"
#include "ComponentManager.hpp"
#include "SystemManager.hpp"


/**
 * @class Coordinator
 * 
 * @todo Make a dedicated page about the Coordinator
 * 
 * The Coordinator is one of the most important classes in
 * the engine. It contains pointers to a ComponentManager,
 * an EntityManager, and a SystemManager, with functions
 * that lead to functionality taken from those classes.
 * 
 * The end user can use this class to create Entities,
 * initialize Components and Systems, call System
 * functions, get Components from an entity, and more.
 * 
 * @author Tim Bishop
*/
class Coordinator
{
public:

	/**
	 * Returns a pointer to the singleton Coordinator.
	 * 
	 * @returns A pointer to the created Coordinator.
	*/
	static Coordinator* Get()
	{
		if (mCoordinatorPtr == nullptr)
		{
			mCoordinatorPtr = new Coordinator();
			mCoordinatorPtr->Init();
		}
		return mCoordinatorPtr;
	}

	/**
	 * Deallocates the Coordinator, provided it
	 * exists.
	*/
	static void DeleteCoordinator()
	{
		if (mCoordinatorPtr != nullptr)
			delete mCoordinatorPtr;
		mCoordinatorPtr = nullptr;
	}

	/**
	 * Initializes the internal Manager classes -
	 * only should be called in Coordinator::Get()
	 * 
	 * @todo Move this to protected/private.
	*/
	void Init()
	{
		// Create pointers to each manager
		mComponentManager = std::make_unique<ComponentManager>();
		mEntityManager = std::make_unique<EntityManager>();
		mSystemManager = std::make_unique<SystemManager>();
	}


	/* ENTITY METHODS */

	
	/**
	 * @copydoc EntityManager::CreateEntity()
	 * 
	 * @param name The string name of the created entity -
	 * should be unique.
	 * 
	 * @returns The newly created Entity
	*/
	Entity CreateEntity(const std::string& name)
	{
		return mEntityManager->CreateEntity(name);
	}

	/**
	 * @copydoc EntityManager::GetEntity()
	*/
	Entity GetEntity(const std::string& name)
	{
		return mEntityManager->GetEntity(name);
	}

	void DestroyEntity(const std::string& name)
	{
		Entity e = mEntityManager->GetEntity(name);
		if (mEntityManager->DestroyEntity(name))
		{
			mComponentManager->EntityDestroyed(e);

			mSystemManager->EntityDestroyed(e);
		}
	}

	void DestroyAllEntities()
	{
		if (mEntityManager->mEntities.empty())
            return;

        std::map<std::string, Entity>::iterator before = mEntityManager->mEntities.begin();
        std::map<std::string, Entity>::iterator after = mEntityManager->mEntities.begin();
        after++;

        while (before != mEntityManager->mEntities.end())
        {
			mComponentManager->EntityDestroyed(before->second);
			mSystemManager->EntityDestroyed(before->second);
            mEntityManager->mSignatures[before->second].reset();
            mEntityManager->mAvailableEntities.push(before->second);
            mEntityManager->mEntities.erase(before);
            before = after;
            if (after != mEntityManager->mEntities.end())
                after++;
        }
	}


	/* COMPONENT METHODS */


	/**
	 * @copydoc ComponentManager::RegisterComponent()
	*/
	template<typename T>
	bool RegisterComponent()
	{
		return mComponentManager->RegisterComponent<T>();
	}

	/**
	 * First attempts to add the component to one of the ComponentManager's
	 * ComponentArray objects. If successful, updates the signature of
	 * the affected Entity, and triggers the System objects to reevaluate
	 * the Entities they affect.
	 * 
	 * @tparam T The type of component to add to the Entity
	 * @param entity The Entity to add a Component to
	 * @param component The Component to add to the Entity
	 * 
	 * @returns True if the entity was successfully added,
	 * false if it was not.
	*/
	template<typename T>
    bool AddComponent(Entity entity, T component)
	{
		if (!mComponentManager->AddComponent<T>(entity, component))
        {
            return false;
        }

		auto signature = mEntityManager->GetSignature(entity);
		signature.set(mComponentManager->GetComponentType<T>(), true);
		mEntityManager->SetSignature(entity, signature);

		mSystemManager->EntitySignatureChanged(entity, signature);
        return true;
	}

	/**
	 * Similar to Coordinator::AddComponent(), but specifically for
	 * the ComponentManager::AddComponentToEntityFromText() method.
	 * 
	 * @param e The entity to add a Component to.
	 * @param typeName The string representation of the Component
	 * subclass.
	*/
	void AddComponentToEntityFromText(Entity e, const std::string& typeName)
	{
		if (!mComponentManager->AddComponentToEntityFromText(e, typeName))
		{
			SPDLOG_ERROR("Could not load component {}", typeName);
			return;
		}

		auto signature = mEntityManager->GetSignature(e);
		signature.set(mComponentManager->GetComponentType(typeName), true);
		mEntityManager->SetSignature(e, signature);

		mSystemManager->EntitySignatureChanged(e, signature);
	}

	/**
	 * Attempts to remove a component from an Entity first. If the
	 * component is successfully removed, change the Entity's signature
	 * and force the System objects to re-initialize their list of
	 * Entities.
	 * 
	 * @tparam T The type of the component to remove
	 * @param entity The Entity to remove the component
	 * from.
	 * 
	 * @returns True if the Component is successfully
	 * removed, or false if it is not.
	*/
	template<typename T>
	bool RemoveComponent(Entity entity)
	{
		if (!mComponentManager->RemoveComponent<T>(entity))
        {
            return false;
        }

		auto signature = mEntityManager->GetSignature(entity);
		signature.set(mComponentManager->GetComponentType<T>(), false);
		mEntityManager->SetSignature(entity, signature);

		mSystemManager->EntitySignatureChanged(entity, signature);
        return true;
	}

	/**
	 * @copydoc ComponentManager::GetComponent()
	*/
	template<typename T>
	T& GetComponent(Entity entity)
	{
		return mComponentManager->GetComponent<T>(entity);
	}

	/**
	 * @copydoc ComponentManager::GetComponentAbstract()
	*/
	Component* GetComponentAbstract(const std::string& typeName, Entity entity)
	{
		return mComponentManager->GetComponentAbstract(typeName, entity);
	}

	/**
	 * @copydoc ComponentManager::GetComponentType()
	*/
	template<typename T>
	ComponentType GetComponentType()
	{
		return mComponentManager->GetComponentType<T>();
	}


	/* SYSTEM METHODS */


	/**
	 * @copydoc SystemManager::RegisterSystem()
	*/
	template<typename T>
	std::shared_ptr<T> RegisterSystem()
	{
		return mSystemManager->RegisterSystem<T>();
	}

	/**
	 * @copydoc SystemManager::GetSystem()
	*/
    template<typename T>
    std::shared_ptr<T> GetSystem()
    {
        return mSystemManager->GetSystem<T>();
    }

	/**
	 * @copydoc SystemManager::SetSignature()
	*/
	template<typename T>
	bool SetSystemSignature(Signature signature)
	{
		return mSystemManager->SetSignature<T>(signature);
	}

protected:
	/** Pointer to the instantiated Coordinator singleton */
	static Coordinator* mCoordinatorPtr;

private:
	std::unique_ptr<ComponentManager> mComponentManager;
	std::unique_ptr<EntityManager> mEntityManager;
	std::unique_ptr<SystemManager> mSystemManager;
};

