#include "Collider.h"
#include "CollisionManager.h"

Collider::Collider() { CollisionManager::PushCollider(this); }
Collider::~Collider() { CollisionManager::PopCollider(this); }