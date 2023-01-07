#pragma once
#include "Collider.h"
#include <list>

class CollisionManager final
{
private:
	static std::list<Collider*> colliders_;

	static bool CheckBoxCollisionPair(Collider* colliderA, Collider* colliderB);
public:
	static void PushCollider(Collider* collider) { colliders_.push_back(collider); }
	static void PopCollider(Collider* collider) { colliders_.remove(collider); }
	static void CheckAllCollisions();
};