#pragma once

#include "../Coordinator.hpp"
#include "../Components/Transform.hpp"
#include "../Components/RectangleCollider.hpp"

class CollisionSystem : public System
{
private:
    int DoCollisionCheck(Transform& t1, RectangleCollider& c1, Transform& t2, RectangleCollider& c2)
    {
        int retval = 0;
        
        if ((t1.x + c1.offsetX < t2.x + c2.offsetX + c2.width) &&
            (t1.x + c1.offsetX + c1.width > t2.x + c2.offsetX) &&
            (t1.y + c1.offsetY < t2.y + c2.offsetY + c2.height) &&
            (t1.y + c1.offsetY + c1.height > t2.y + c2.offsetY))
        {
            retval = 15;
        }

        return retval;
    }

public:
    void Do()
    {
        Coordinator* cd = Coordinator::Get();
        std::set<Entity>::iterator first;
        std::set<Entity>::iterator second;
        for (first = mEntities.begin(); first != mEntities.end(); first++)
        {
            Transform& first_transform = cd->GetComponent<Transform>(*first);
            RectangleCollider& first_collider = cd->GetComponent<RectangleCollider>(*first);

            second = first;
            second++;
            for (; second != mEntities.end(); second++)
            {
                Transform& second_transform = cd->GetComponent<Transform>(*second);
                RectangleCollider& second_collider = cd->GetComponent<RectangleCollider>(*second);

                int retval = DoCollisionCheck(first_transform, first_collider, second_transform, second_collider);
                if (retval == 0) continue;

                Collision c;
                c.ent_collided = *second;
                c.collision_pos = retval;
                first_collider.collisions.emplace_back(c);

                retval = (~retval) & 15;
                c.ent_collided = *first;
                c.collision_pos = retval;
                second_collider.collisions.emplace_back(c);
            }
        }
    }

    void Clear()
    {
        Coordinator* cd = Coordinator::Get();
        for (Entity e : mEntities)
        {
            RectangleCollider& c = cd->GetComponent<RectangleCollider>(e);
            c.collisions.clear();
        }
    }

    Signature GetSignature() override
    {
        Signature sig;
        Coordinator* cd = Coordinator::Get();
        sig[cd->GetComponentType<Transform>()].flip();
        sig[cd->GetComponentType<RectangleCollider>()].flip();
        return sig;
    }
};