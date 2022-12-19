#pragma once
#include <list>
#include "Collider.h"
#include <memory>

class CollisionManager
{
private:
	bool CheckBoxCollisionPair(Collider* colliderA, Collider* colliderB);
public:
	void CheckAllCollisions();
};