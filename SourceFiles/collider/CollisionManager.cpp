#include "CollisionManager.h"
#include <cassert>
#include <imgui.h>
using namespace std;

list<BoxCollider*> CollisionManager::boxColliders;
list<SphereCollider*> CollisionManager::sphereColliders;
list<PlaneCollider*> CollisionManager::planeColliders;
list<PolygonCollider*> CollisionManager::triangleColliders;
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
	const float epsilon = 1.0e-5f; // �덷�z���p�̔����Ȓl
	// �ʖ@���ƃ��C�̕����x�N�g���̓���
	float d1 = colliderB->GetNormal().dot(colliderA->GetRayDirection());
	// ���ʂɂ͓�����Ȃ�
	if (d1 > -epsilon) { return false; }
	// �n�_�ƌ��_�̋���(���ʂ̖@������)
	// �ʖ@���ƃ��C�̎n�_���W(�ʒu�x�N�g��)�̓���
	float d2 = colliderB->GetNormal().dot(colliderA->GetWorldPosition());
	// �n�_�ƕ��ʂ̋���(���ʂ̖@������)
	float dist = d2 - colliderB->GetDistance();
	// �n�_�ƕ��ʂ̋���(���C����)
	float t = dist / -d1;
	// ��_���n�_�����ɂ���̂œ�����Ȃ�
	if (t < 0) { return false; }
	// ��������������
	if (distance) { *distance = t; }
	// ��_���v�Z
	colliderB->SetInter(colliderA->GetWorldPosition() + t * colliderA->GetRayDirection());
	return true;
}

bool CollisionManager::CheckCollisionRayPolygon(RayCollider* colliderA, PolygonCollider* colliderB, float* distance)
{
	if (!CheckCollisionFiltering(colliderA, colliderB)) { return false; }
	// �O�p�`������Ă��镽�ʂ��Z�o
	colliderB->ComputeDistance();
	// ���C�ƕ��ʂ��������Ă��Ȃ���Γ����Ă��Ȃ�
	PlaneCollider tempColliderB;
	colliderB->ToPlaneCollider(&tempColliderB);

	if (!CheckCollisionRayPlane(colliderA, &tempColliderB, distance)) { return false; }
	// ���C�ƕ��ʂ��������Ă����̂ŁA�����ƍ��W���������܂ꂽ
	// ���C�ƕ��ʂ̌�_���O�p�`�̓����ɂ��邩����
	const float epsilon = 1.0e-5f; // �덷�z���p�̔����Ȓl

	size_t vertexSize = colliderB->GetVertices().size();
	// ���_����2�ȉ��Ȃ�G���[
	assert(vertexSize > 2);

	for (size_t i = 0; i < vertexSize; i++)
	{
		// ��pi_p(i+1)�ɂ���
		Vector3 pt_px = colliderB->GetVertices()[i] - *tempColliderB.GetInter();
		Vector3 px_py = colliderB->GetVertices()[(i + 1) % vertexSize] - colliderB->GetVertices()[i];
		Vector3 m = pt_px.cross(px_py);
		// �ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
		if (m.dot(colliderB->GetNormal()) < -epsilon) { return false; }
	}

	// �����Ȃ̂œ������Ă���
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

			// �v���C���[�������ɓ��������瓖���蔻�菈���𔲂���
			if (boxColliderA->GetCollisionAttribute() == CollisionAttribute::Player ||
				boxColliderB->GetCollisionAttribute() == CollisionAttribute::Player)
			{
				return;
			}
		}
		if (boxColliderA->GetCollisionAttribute() == CollisionAttribute::Player) { return; }
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
		for (PolygonCollider* triangleCollider : triangleColliders)
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
	ImGui::Text("boxColliders.size() = %d", boxColliders.size());
	ImGui::Text("planeColliders.size() = %d", planeColliders.size());
	ImGui::Text("triangleColliders.size() = %d", triangleColliders.size());
}