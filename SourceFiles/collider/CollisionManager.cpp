#include "CollisionManager.h"
#include <cassert>
#include <imgui.h>
#include <algorithm>
using namespace std;

list<BoxCollider*> CollisionManager::boxColliders;
list<IncludeCollider*> CollisionManager::includeColliders;
list<SphereCollider*> CollisionManager::sphereColliders;
list<PlaneCollider*> CollisionManager::planeColliders;
list<PolygonCollider*> CollisionManager::polygonColliders;
list<RayCollider*> CollisionManager::rayColliders;

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

bool CollisionManager::CheckCollision2IncludeObjects(IncludeCollider* colliderA, IncludeCollider* colliderB)
{
	if (!CheckCollisionFiltering(colliderA, colliderB)) { return false; }

	Vector3 vecAB = colliderA->GetWorldPosition() - colliderB->GetWorldPosition();
	// 使用フラグの取得
	array<bool, 3> isUse{};
	
	for (size_t i = 0; i < isUse.size(); i++)
	{
		isUse[i] = colliderA->GetUseAxis()[i] && colliderB->GetUseAxis()[i];
	}

	// 計算に使わない値を0にする
	if (!isUse[(size_t)IncludeCollider::Axis::X]) { vecAB.x = 0; }
	if (!isUse[(size_t)IncludeCollider::Axis::Y]) { vecAB.y = 0; }
	if (!isUse[(size_t)IncludeCollider::Axis::Z]) { vecAB.z = 0; }

	return vecAB.length() <= IncludeCollider::GetIncludeRadius();
}

bool CollisionManager::CheckCollision2Spheres(SphereCollider* colliderA, SphereCollider* colliderB)
{
	if (!CheckCollisionFiltering(colliderA, colliderB)) { return false; }

	Vector3 vecAB = colliderA->GetWorldPosition() - colliderB->GetWorldPosition();
	float radAB = colliderA->GetRadius() + colliderB->GetRadius();

	return vecAB.length() <= radAB;
}

bool CollisionManager::CheckCollisionRayPlane(RayCollider* colliderA, PlaneCollider* colliderB, float* distance)
{
	if (!CheckCollisionFiltering(colliderA, colliderB)) { return false; }
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
	colliderB->SetInter(colliderA->GetWorldPosition() + t * colliderA->GetRayDirection());
	return true;
}

bool CollisionManager::CheckCollisionRayPolygon(RayCollider* colliderA, PolygonCollider* colliderB, float* distance)
{
	if (!CheckCollisionFiltering(colliderA, colliderB)) { return false; }
	// 三角形が乗っている平面を算出
	colliderB->ComputeDistance();
	// レイと平面が当たっていなければ当っていない
	PlaneCollider tempColliderB;
	colliderB->ToPlaneCollider(&tempColliderB);

	if (!CheckCollisionRayPlane(colliderA, &tempColliderB, distance)) { return false; }
	// レイと平面が当たっていたので、距離と座標が書き込まれた
	// レイと平面の交点が三角形の内側にあるか判定
	const float epsilon = 1.0e-5f; // 誤差吸収用の微小な値

	size_t vertexSize = colliderB->GetVertices().size();
	// 頂点数が2つ以下ならエラー
	assert(vertexSize > 2);

	for (size_t i = 0; i < vertexSize; i++)
	{
		// 辺pi_p(i+1)について
		Vector3 pt_px = colliderB->GetVertices()[i] - *tempColliderB.GetInter();
		Vector3 px_py = colliderB->GetVertices()[(i + 1) % vertexSize] - colliderB->GetVertices()[i];
		Vector3 m = pt_px.cross(px_py);
		// 辺の外側であれば当たっていないので判定を打ち切る
		if (m.dot(colliderB->GetNormal()) < -epsilon) { return false; }
	}

	// 内側なので当たっている
	return true;
}

void CollisionManager::CheckBoxCollisions()
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

void CollisionManager::CheckIncludeCollisions()
{
	for (IncludeCollider* includeBoxColliderA : includeColliders) {
		for (IncludeCollider* includeBoxColliderB : includeColliders)
		{
			if (includeBoxColliderA == includeBoxColliderB) { continue; }
			if (!CheckCollision2IncludeObjects(includeBoxColliderA, includeBoxColliderB)) { continue; }

			includeBoxColliderA->OnCollision(includeBoxColliderB);
			includeBoxColliderB->OnCollision(includeBoxColliderA);
		}
	}
}

void CollisionManager::CheckSphereCollisions()
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

void CollisionManager::CheckRayPlaneCollisions()
{
	for (RayCollider* rayCollider : rayColliders) {
		for (PlaneCollider* planeCollider : planeColliders)
		{
			if (!CheckCollisionRayPlane(rayCollider, planeCollider)) { continue; }

			rayCollider->OnCollision(planeCollider);
			planeCollider->OnCollision(rayCollider);
		}
	}
}

void CollisionManager::CheckRayPolygonCollisions()
{
	for (RayCollider* rayCollider : rayColliders) {
		for (PolygonCollider* triangleCollider : polygonColliders)
		{
			if (!CheckCollisionRayPolygon(rayCollider, triangleCollider)) { continue; }

			rayCollider->OnCollision(triangleCollider);
			triangleCollider->OnCollision(rayCollider);
		}
	}
}

void CollisionManager::CheckAllCollisions()
{
	CheckBoxCollisions();
	CheckRayPlaneCollisions();
	CheckRayPolygonCollisions();
	CheckIncludeCollisions();
	ImGui::Text("boxColliders.size() = %d", boxColliders.size());
	ImGui::Text("planeColliders.size() = %d", planeColliders.size());
	ImGui::Text("polygonColliders.size() = %d", polygonColliders.size());
	ImGui::Text("includeColliders.size() = %d", includeColliders.size());
}