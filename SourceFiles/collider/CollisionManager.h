#pragma once
#include "Collider.h"
#include <list>

class CollisionManager final
{
private:
	static std::list<BoxCollider*> boxColliders;
	static std::list<IncludeCollider*> includeColliders;
	static std::list<SphereCollider*> sphereColliders;
	static std::list<PlaneCollider*> planeColliders;
	static std::list<PolygonCollider*> polygonColliders;
	static std::list<RayCollider*> rayColliders;

public:
	static bool CheckCollisionFiltering(BaseCollider* colliderA, BaseCollider* colliderB);
	static bool CheckCollision2Boxes(BoxCollider* colliderA, BoxCollider* colliderB);
	static bool CheckCollision2IncludeObjects(IncludeCollider* colliderA, IncludeCollider* colliderB);
	static bool CheckCollision2Spheres(SphereCollider* colliderA, SphereCollider* colliderB);
	static bool CheckCollisionRayPlane(RayCollider* colliderA, PlaneCollider* colliderB, float* distance = nullptr);
	static bool CheckCollisionRayPolygon(RayCollider* colliderA, PolygonCollider* colliderB, float* distance = nullptr);

	CollisionManager() = delete;
	// �R���C�_�[�o�^�֐�
	static void PushCollider(BoxCollider* collider) { boxColliders.push_back(collider); }
	static void PushCollider(IncludeCollider* collider) { includeColliders.push_back(collider); }
	static void PushCollider(SphereCollider* collider) { sphereColliders.push_back(collider); }
	static void PushCollider(PlaneCollider* collider) { planeColliders.push_back(collider); }
	static void PushCollider(PolygonCollider* collider) { polygonColliders.push_back(collider); }
	static void PushCollider(RayCollider* collider) { rayColliders.push_back(collider); }
	static void PopCollider(BoxCollider* collider) { boxColliders.remove(collider); }
	static void PopCollider(IncludeCollider* collider) { includeColliders.remove(collider); }
	static void PopCollider(SphereCollider* collider) { sphereColliders.remove(collider); }
	static void PopCollider(PlaneCollider* collider) { planeColliders.remove(collider); }
	static void PopCollider(PolygonCollider* collider) { polygonColliders.remove(collider); }
	static void PopCollider(RayCollider* collider) { rayColliders.remove(collider); }
	// �ʓ����蔻��
	static void CheckBoxCollisions();
	static void CheckIncludeCollisions();
	static void CheckSphereCollisions();
	static void CheckRayPlaneCollisions();
	static void CheckRayPolygonCollisions();
	// �S�����蔻��
	static void CheckAllCollisions();
};