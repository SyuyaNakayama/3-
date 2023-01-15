#include "Collider.h"
#include "CollisionManager.h"

BoxCollider::BoxCollider() { CollisionManager::PushCollider(this); }
BoxCollider::~BoxCollider() { CollisionManager::PopCollider(this); }
SphereCollider::SphereCollider() { CollisionManager::PushCollider(this); }
SphereCollider::~SphereCollider() { CollisionManager::PopCollider(this); }
RayCollider::RayCollider() { CollisionManager::PushCollider(this); }
RayCollider::~RayCollider() { CollisionManager::PopCollider(this); }
PolygonCollider::PolygonCollider() { CollisionManager::PushCollider(this); }
PolygonCollider::~PolygonCollider() { CollisionManager::PopCollider(this); }
PlaneCollider::PlaneCollider() { CollisionManager::PushCollider(this); }
PlaneCollider::~PlaneCollider() { CollisionManager::PopCollider(this); }

void PolygonCollider::SetVertices()
{
	Vector3 objPos = worldTransform.translation_;
	Vector3 objRad = worldTransform.scale_;
	vertices.push_back(objPos + Vector3(-objRad.x, objRad.y, -objRad.z));
	vertices.push_back(objPos + objRad);
	vertices.push_back(objPos + Vector3(objRad.x, -objRad.y, -objRad.z));
	vertices.push_back(objPos - objRad);
}