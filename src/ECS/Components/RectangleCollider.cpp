#include "ECS/Components/RectangleCollider.hpp"

bool CheckCollision(Collision& c, int position)
{
    return (c.collision_pos & position) == position;
}

void AddCollision(Collision& c, int position)
{
    c.collision_pos |= position;
}