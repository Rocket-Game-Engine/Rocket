#pragma once

/**
 * @file Entity.h
 * 
 * A simple set of definitions corresponding to
 * the Entity.
 * 
 * @todo Take out Transform and implement a simple
 * Entity class? It would be weird if we do that.
 * 
 * @note This, and most base ECS classes, were adapted from
 * [Austin Morlan's amazing ECS writeup here.](https://austinmorlan.com/posts/entity_component_system/)
 * 
 * @author Tim Bishop
*/

#include <cstdint>

using Entity = std::uint32_t;

/** The maximum number of entities allowed in the scene. */
const Entity MAX_ENTITIES = 5000;
