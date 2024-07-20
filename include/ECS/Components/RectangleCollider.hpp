#pragma once

#include "../Coordinator.hpp"
#include <vector>

const int COLLISION_LEFT = 1;
const int COLLISION_TOP = 2;
const int COLLISION_RIGHT = 4;
const int COLLISION_BOTTOM = 8;

struct Collision
{
    int collision_pos;
    Entity ent_collided;
};

bool CheckCollision(Collision& c, int position);

void AddCollision(Collision& c, int position);

ROCKET_COMPONENT(RectangleCollider,
    ROCKET_PROPERTY_DEFVAL(public, double, offsetX, 0)
    ROCKET_PROPERTY_DEFVAL(public, double, offsetY, 0)
    ROCKET_PROPERTY(public, double, width)
    ROCKET_PROPERTY(public, double, height)
    ROCKET_RAW(public: std::vector<Collision> collisions;)
);
