#pragma once
#include "Collider.h"
#include <list>
#include <Collision.cpp>

class CollisionManager final
{
private:
	static std::list<BoxCollider*> boxColliders_;
	static std::list<SphereCollider*> sphereColliders_;
	static std::list<PlaneCollider*> planeColliders_;
	static std::list<TriangleCollider*> triangleColliders_;
	static std::list<RayCollider*> rayColliders_;

	static bool CheckCollisionFiltering(BaseCollider* colliderA, BaseCollider* colliderB);
	static bool CheckCollision2Boxes(BoxCollider* colliderA, BoxCollider* colliderB);
	static bool CheckCollision2Spheres(SphereCollider* colliderA, SphereCollider* colliderB);
	//static bool CheckCollisionSpherePlane(const Sphere& sphere, const Plane& plane, Vector3* inter = nullptr);
	//static bool CheckCollisionSphereTriangle(const Sphere& sphere, const Triangle& triangle, Vector3* inter = nullptr);
	static bool CheckCollisionRayPlane(RayCollider* colliderA, PlaneCollider* colliderB, float* distance = nullptr, Vector3* inter = nullptr);
	static bool CheckCollisionRayTriangle(RayCollider* colliderA, TriangleCollider* colliderB, float* distance = nullptr, Vector3* inter = nullptr);
	//static bool CheckCollisionRayRectangle(const Ray& ray, const Triangle& triangle, float* distance = nullptr, Vector3* inter = nullptr);
	//static bool CheckCollisionRaySphere(const Ray& ray, const Sphere& sphere, float* distance = nullptr, Vector3* inter = nullptr);

	static void CheckCollisions(std::list<BoxCollider*>& boxColliders);
	static void CheckCollisions(std::list<SphereCollider*>& sphereColliders);
	//static void CheckCollisions(std::list<PlaneCollider*> planeColliders);
	static void CheckCollisions(std::list<RayCollider*> rayColliders, std::list<TriangleCollider*> triangleColliders);
public:
	CollisionManager() = delete;
	static void PushCollider(BoxCollider* collider) { boxColliders_.push_back(collider); }
	static void PushCollider(SphereCollider* collider) { sphereColliders_.push_back(collider); }
	static void PushCollider(PlaneCollider* collider) { planeColliders_.push_back(collider); }
	static void PushCollider(TriangleCollider* collider) { triangleColliders_.push_back(collider); }
	static void PushCollider(RayCollider* collider) { rayColliders_.push_back(collider); }
	static void PopCollider(BoxCollider* collider) { boxColliders_.remove(collider); }
	static void PopCollider(SphereCollider* collider) { sphereColliders_.remove(collider); }
	static void PopCollider(PlaneCollider* collider) { planeColliders_.remove(collider); }
	static void PopCollider(TriangleCollider* collider) { triangleColliders_.remove(collider); }
	static void PopCollider(RayCollider* collider) { rayColliders_.remove(collider); }
	static void CheckAllCollisions();
};