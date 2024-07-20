#ifndef _ROC_COMPONENT_MANAGER_H_
#define _ROC_COMPONENT_MANAGER_H_

/**
 * @file ComponentManager.h
 * 
 * This file defines the class ComponentManager, a
 * class explicitly designed to store and keep track of
 * the Component objects created and attached to
 * Entities.
 * 
 * @note This, and most base ECS classes, were adapted from
 * [Austin Morlan's amazing ECS writeup here.](https://austinmorlan.com/posts/entity_component_system/)
 * 
 * @todo A LOT of these functions could use a cleanup, especially
 * in the error checking department.
 * 
 * @author Tim Bishop
*/

#include <memory>
#include <functional>
#include <string>
#include <spdlog/spdlog.h>

#include "IComponentArray.hpp"


/**
 * @class ComponentManager
 * 
 * This class keeps all the ComponentArray objects created
 * over the course of the Engine's runtime. They also
 * facilitate the Coordinator retrieving, creating, and
 * erasing Component objects.
*/
class ComponentManager
{
public:
	/**
	 * This method does exactly what one would expect:
	 * it adds a new Component of type T to the Entity
	 * provided to it.
	 * 
	 * @tparam T The subclass of Component to instantiate.
	 * @param entity The Entity to attach the new T object to.
	 * 
	 * @return True if the component was successfully added,
	 * false if there was some error.
	*/
	template<typename T>
	bool AddComponent(Entity entity)
	{
		// Add a component to the array for an entity
        std::shared_ptr<ComponentArray<T>> ptr = GetComponentArray<T>();
        if (ptr == nullptr) {SPDLOG_WARN("Couldn't get a ComponentArray for {}", T::name()); return false;}
		return ptr->InsertData(entity, T());
	}

	/**
	 * This method does exactly what one would expect:
	 * it adds a new Component of type T to the Entity
	 * provided to it. Unlike the other definition
	 * of AddComponent, this one asks for an already
	 * created instance of the T object, which it will
	 * copy into place in the ComponentArray.
	 * 
	 * @tparam T The subclass of Component to instantiate.
	 * @param entity The Entity to attach the new T object to.
	 * @param component The instance of T to add to the entity.
	 * 
	 * @return True if the component was successfully added,
	 * false if there was some error.
	*/
	template<typename T>
	bool AddComponent(Entity entity, T component)
	{
		// Add a component to the array for an entity
        std::shared_ptr<ComponentArray<T>> ptr = GetComponentArray<T>();
        if (ptr == nullptr) { throw std::runtime_error("Attempted to add to a nonexistent ComponentArray!"); }
		return ptr->InsertData(entity, component);
	}

	/**
	 * This function registers the given class
	 * as a Component in the eyes of the Engine. After calling
	 * this function, a ComponentArray of template type T
	 * will be instantiated, and all commands relating to
	 * adding, erasing, and accessing the new type will work
	 * correctly.
	 * 
	 * @note Only add types subclassed from Component to the
	 * Engine, unless you REALLY know what you're doing.
	 * 
	 * @tparam T The new component type to be added.
	 * 
	 * @returns True if the component was added correctly,
	 * false if it wasn't.
	*/
	template<typename T>
	bool RegisterComponent()
	{
		std::string typeName = T::name();

		if (mComponentTypes.find(typeName) != mComponentTypes.end())
        {
            SPDLOG_ERROR("Attempting to register ComponentType twice.");
            return false;
        }

		// Add this component type to the component type map
		mComponentTypes.insert({typeName, mNextComponentType});

		// Create a ComponentArray pointer and add it to the component arrays map
		mComponentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

		mCreateCompFuncs[typeName] = [=](Entity e){ return this->AddComponent<T>(e); };

		mAccessCompFuncs[typeName] = [=](Entity e){ return (Component*)this->GetComponentPtr<T>(e); };

		// Increment the value so that the next component registered will be different
		++mNextComponentType;
        return true;
	}

	/**
	 * This function is used in LoadScene() exclusively
	 * to instantiate a Component subclass based on its name.
	 * This should not be used in normal code.
	 * 
	 * @todo Perhaps add LoadScene() as a friend function, and
	 * make this function private? (Or similar in the Coordinator?)
	 * 
	 * @param e The entity to add the new Component to.
	 * @param typeName The string representation of the Component.
	 * 
	 * @returns True if the Component was added successfully, false
	 * if it was not.
	*/
	bool AddComponentToEntityFromText(Entity e, const std::string& typeName)
	{
		if (mCreateCompFuncs.find(typeName) == mCreateCompFuncs.end())
		{
			throw std::runtime_error(std::string("Could not find component ") + typeName);
		}

		return mCreateCompFuncs[typeName](e);
	}

	/**
	 * Returns the unsigned integer representing the Component's
	 * internal type.
	 * 
	 * @tparam T The component to check for.
	 * 
	 * @returns MAX_COMPONENTS if the Component was not
	 * already registered, or the ComponentType corresponding
	 * to it if it has been.
	*/
	template<typename T>
	ComponentType GetComponentType()
	{
		std::string typeName = T::name();

		if (mComponentTypes.find(typeName) == mComponentTypes.end())
        {
            SPDLOG_ERROR("Attempted to access ComponentType before registering.");
            return MAX_COMPONENTS;
        }

		// Return this component's type - used for creating signatures
		return mComponentTypes[typeName];
	}

	/**
	 * Similar to GetComponentType(), but used exclusively
	 * for the string representation of the class. Used in
	 * LoadScene().
	 * 
	 * @param typeName The string representation of the Component
	 * subclass.
	 * 
	 * @returns MAX_COMPONENTS if the Component was not
	 * already registered, or the ComponentType corresponding
	 * to it if it has been.
	*/
	ComponentType GetComponentType(const std::string& typeName)
	{
		if (mComponentTypes.find(typeName) == mComponentTypes.end())
        {
            SPDLOG_ERROR("Attempted to access ComponentType before registering.");
            return MAX_COMPONENTS;
        }

		// Return this component's type - used for creating signatures
		return mComponentTypes[typeName];
	}

	/**
	 * Removes the Component of type T from the given Entity.
	 * 
	 * @tparam T The subclass of Component to search for.
	 * @param entity The entity to remove the Component from.
	 * 
	 * @returns True if the Component was removed, false if
	 * it was not.
	*/
	template<typename T>
	bool RemoveComponent(Entity entity)
	{
		// Remove a component from the array for an entity
        std::shared_ptr<ComponentArray<T>> ptr = GetComponentArray<T>();
        if (ptr == nullptr) { throw std::runtime_error("Attempted to remove pointer from nonexistent ComponentArray!"); }
		return ptr->RemoveData(entity);
	}

	/**
	 * This method returns a reference to the subclass of Component,
	 * T, currently attached to the given Entity
	 * 
	 * @tparam T The subclass of Component to search for.
	 * @param entity The entity to get the Component of.
	 * 
	 * @returns A reference to the entity's component.
	*/
	template<typename T>
	T& GetComponent(Entity entity)
	{
		// Get a reference to a component from the array for an entity
        std::shared_ptr<ComponentArray<T>> ptr = GetComponentArray<T>();
        /** @todo AGAIN PLEASE FIX, I HATE ASSERTS IN REAL CODE */
        if (ptr == nullptr)
		{
			throw std::runtime_error("Attempted to get data from a component array that hasn't been initialized!");
		}
		return ptr->GetData(entity);
	}

	/**
	 * A method called exclusively in LoadScene(), this method
	 * returns a Component pointer to the Component subclass
	 * of type `typename` at index `entity`.
	 * 
	 * @param typeName A string representation of the subclass of Component.
	 * @param entity The entity to get the Component of.
	 * 
	 * @returns A pointer to a Component object somewhere in the
	 * ComponentArray objects, or nullptr if the type doesn't exist.
	*/
	Component* GetComponentAbstract(const std::string& typeName, Entity entity)
	{
		if (mAccessCompFuncs.find(typeName) == mAccessCompFuncs.end())
        {
            SPDLOG_ERROR("Attempted to access Component before registering.");
            return nullptr;
        }
		return mAccessCompFuncs.at(typeName)(entity);
	}

	/**
	 * A function meant to be triggered when an Entity is destroyed.
	 * It will be called by the Coordinator, and when it is, we
	 * loop through each ComponentArray object inside this Manager
	 * and remove the Component at index `entity`.
	 * 
	 * @param entity The entity that was just destroyed.
	*/
	void EntityDestroyed(Entity entity)
	{
		// Notify each component array that an entity has been destroyed
		// If it has a component for that entity, it will remove it
		for (auto const& pair : mComponentArrays)
		{
			auto const& component = pair.second;

			component->EntityDestroyed(entity);
		}
	}

private:
	/** Map from type string pointer to a component type */
	std::unordered_map<std::string, ComponentType> mComponentTypes{};

	/** Map from type string pointer to a component array */
	std::unordered_map<std::string, std::shared_ptr<IComponentArray>> mComponentArrays{};

	/** Map from string name of Components to their Adder function */
	std::unordered_map<std::string, std::function<bool(Entity)>> mCreateCompFuncs{};

	/** Map from string name of Components to a function returning a pointer to a Component */
	std::unordered_map<std::string, std::function<Component*(Entity)>> mAccessCompFuncs{};

	/** The component type to be assigned to the next registered component - starting at 0 */
	ComponentType mNextComponentType{};

	/**
	 * Convenience function to get the statically casted pointer to the ComponentArray of type T.
	 * 
	 * @tparam T The subclass of Component.
	 * @return A shared_ptr to the ComponentArray containing the class
	 * of Component defined in T, or nullptr if it doesn't exist.
	 */
	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray()
	{
		std::string typeName = T::name();

		if (mComponentTypes.find(typeName) == mComponentTypes.end())
        {
            SPDLOG_ERROR("Attempted to access nonexistent ComponentArray.");
            return nullptr;
        }

		return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
	}

	/**
	 * Convenience function for accessing the abstract version of a component.
	 * 
	 * @tparam T The subclass of Component to access.
	 * @param e The Entity to return the Component of.
	 * 
	 * @returns A pointer to the given component of the entity.
	 */
	template<typename T>
	T* GetComponentPtr(Entity e)
	{
		std::string typeName = T::name();

		std::shared_ptr<ComponentArray<T>> c_ar = GetComponentArray<T>();

		return &(c_ar.get()->GetData(e));
	}
};

#endif
