#include "CollisionManager.h"
#include <cassert>
using namespace std;

std::list<BoxCollider*> CollisionManager::boxColliders_;
std::list<SphereCollider*> CollisionManager::sphereColliders_;
std::list<PlaneCollider*> CollisionManager::planeColliders_;
std::list<PolygonCollider*> CollisionManager::triangleColliders_;
std::list<RayCollider*> CollisionManager::rayColliders_;

bool CollisionManager::CheckCollisionFiltering(BaseCollider* colliderA, BaseCollider* colliderB)
{
	return 
		(UINT)colliderA->GetCollisionAttribute() & (UINT)colliderB->GetCollisionMask() ||
		(UINT)colliderB->GetCollisionAttribute() & (UINT)colliderA->GetCollisionMask();
}

bool CollisionManager::CheckCollision2Boxes(BoxCollider* colliderA, BoxCollider* colliderB)
{
	if (!CheckCollisionFiltering(colliderA, colliderB)) { return false; }

	Vector3 vecAB = colliderA->GetWorldPosition() - colliderB->GetWorldPosition();
	Vector3 radAB = colliderA->GetRadius() + colliderB->GetRadius();
	vecAB = vecAB.abs();

	return vecAB <= radAB;
}

bool CollisionManager::CheckCollision2Spheres(SphereCollider* colliderA, SphereCollider* colliderB)
{
	if (!CheckCollisionFiltering(colliderA, colliderB)) { return false; }

	Vector3 vecAB = colliderA->GetWorldPosition() - colliderB->GetWorldPosition();
	float radAB = colliderA->GetRadius() + colliderB->GetRadius();

	return vecAB.length() <= radAB; 
}

bool CollisionManager::CheckCollisionRayPlane(RayCollider* colliderA, PlaneCollider* colliderB, float* distance, Vector3* inter)
{
	const float epsilon = 1.0e-5f; // 誤差吸収用の微小な値
	// 面法線とレイの方向ベクトルの内積
	float d1 = colliderB->GetNormal().dot(colliderA->GetRayDirection());
	// 裏面には当たらない
	if (d1 > -epsilon) { return false; }
	// 始点と原点の距離(平面の法線方向)
	// 面法線とレイの始点座標(位置ベクトル)の内積
	float d2 = colliderB->GetNormal().dot(colliderA->GetWorldPosition());
	// 始点と平面の距離(平面の法線方向)
	float dist = d2 - colliderB->GetDistance();
	// 始点と平面の距離(レイ方向)
	float t = dist / -d1;
	// 交点が始点より後ろにあるので当たらない
	if (t < 0) { return false; }
	// 距離を書き込む
	if (distance) { *distance = t; }
	// 交点を計算
	if (inter) { *inter = colliderA->GetWorldPosition() + t * colliderA->GetRayDirection(); }
	return true;
}

bool CollisionManager::CheckCollisionRayTriangle(RayCollider* colliderA, PolygonCollider* colliderB, float* distance, Vector3* inter)
{
	// 三角形が乗っている平面を算出
	Vector3 interPlane;
	colliderB->ComputeDistance();
	// レイと平面が当たっていなければ当っていない
	if (!CheckCollisionRayPlane(colliderA, colliderB, distance, &interPlane)) { return false; }
	// レイと平面が当たっていたので、距離と座標が書き込まれた
	// レイと平面の交点が三角形の内側にあるか判定
	const float epsilon = 1.0e-5f; // 誤差吸収用の微小な値

	size_t vertexSize = colliderB->GetVertices().size();
	// 頂点数が2つ以下ならエラー
	assert(vertexSize > 2);

	for (size_t i = 0; i < vertexSize; i++)
	{
		// 辺pi_p(i+1)について
		Vector3 pt_px = colliderB->GetVertices()[i] - interPlane;
		Vector3 px_py = colliderB->GetVertices()[(i + 1) % vertexSize] - colliderB->GetVertices()[i];
		Vector3 m = pt_px.cross(px_py);
		// 辺の外側であれば当たっていないので判定を打ち切る
		if (m.dot(colliderB->GetNormal()) < -epsilon) { return false; }
	}

	// 内側なので当たっている
	if (inter) { *inter = interPlane; }

	return true;
}

void CollisionManager::CheckCollisions(std::list<BoxCollider*>& boxColliders)
{
	for (BoxCollider* boxColliderA : boxColliders) {
		for (BoxCollider* boxColliderB : boxColliders)
		{
			if (boxColliderA == boxColliderB) { continue; }
			if (!CheckCollision2Boxes(boxColliderA, boxColliderB)) { continue; }

			boxColliderA->OnCollision(boxColliderB);
			boxColliderB->OnCollision(boxColliderA);

			// プレイヤーが何かに当たったら当たり判定処理を抜ける
			if (boxColliderA->GetCollisionAttribute() == CollisionAttribute::Player ||
				boxColliderB->GetCollisionAttribute() == CollisionAttribute::Player)
			{
				return;
			}
		}
		if (boxColliderA->GetCollisionAttribute() == CollisionAttribute::Player) { return; }
	}
}

void CollisionManager::CheckCollisions(std::list<SphereCollider*>& sphereColliders)
{
	for (SphereCollider* sphereColliderA : sphereColliders) {
		for (SphereCollider* sphereColliderB : sphereColliders)
		{
			if (sphereColliderA == sphereColliderB) { continue; }
			if (!CheckCollision2Spheres(sphereColliderA, sphereColliderB)) { continue; }

			sphereColliderA->OnCollision(sphereColliderB);
			sphereColliderB->OnCollision(sphereColliderA);
		}
	}
}

void CollisionManager::CheckCollisions(std::list<RayCollider*> rayColliders, std::list<PolygonCollider*> triangleColliders)
{
	for (RayCollider* rayCollider : rayColliders) {
		for (PolygonCollider* triangleCollider : triangleColliders)
		{
			if (!CheckCollisionRayTriangle(rayCollider, triangleCollider)) { continue; }

			rayCollider->OnCollision(triangleCollider);
			triangleCollider->OnCollision(rayCollider);
		}
	}
}

void CollisionManager::CheckAllCollisions()
{
	CheckCollisions(boxColliders_);
	CheckCollisions(rayColliders_, triangleColliders_);
}