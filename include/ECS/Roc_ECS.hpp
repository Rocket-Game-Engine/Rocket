#ifndef _ROC_ECS_H_
#define _ROC_ECS_H_

/**
 * @file Roc_ECS.h
 * 
 * @brief Overall include for the Entity-Component System
 * 
 * @note This, and most base ECS classes, were adapted from
 * [Austin Morlan's amazing ECS writeup here.](https://austinmorlan.com/posts/entity_component_system/)
*/

// Core

#include "Entity.hpp"
#include "Component.hpp"
#include "EntityManager.hpp"
#include "ComponentManager.hpp"
#include "SystemManager.hpp"
#include "Coordinator.hpp"

// Components

#include "Components/Transform.hpp"
#include "Components/Gravity.hpp"
#include "Components/Sprite.hpp"
#include "Components/RectangleCollider.hpp"

// Systems

#include "Systems/RenderSpriteSys.hpp"
#include "Systems/CollisionSystem.hpp"

#endif