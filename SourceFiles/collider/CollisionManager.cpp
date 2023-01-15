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
	if (inter) { *inter = colliderA->GetWorldPosition() + t * colliderA->GetRayDirection(); }
	return true;
}

bool CollisionManager::CheckCollisionRayTriangle(RayCollider* colliderA, PolygonCollider* colliderB, float* distance, Vector3* inter)
{
	// �O�p�`������Ă��镽�ʂ��Z�o
	Vector3 interPlane;
	colliderB->ComputeDistance();
	// ���C�ƕ��ʂ��������Ă��Ȃ���Γ����Ă��Ȃ�
	if (!CheckCollisionRayPlane(colliderA, colliderB, distance, &interPlane)) { return false; }
	// ���C�ƕ��ʂ��������Ă����̂ŁA�����ƍ��W���������܂ꂽ
	// ���C�ƕ��ʂ̌�_���O�p�`�̓����ɂ��邩����
	const float epsilon = 1.0e-5f; // �덷�z���p�̔����Ȓl

	size_t vertexSize = colliderB->GetVertices().size();
	// ���_����2�ȉ��Ȃ�G���[
	assert(vertexSize > 2);

	for (size_t i = 0; i < vertexSize; i++)
	{
		// ��pi_p(i+1)�ɂ���
		Vector3 pt_px = colliderB->GetVertices()[i] - interPlane;
		Vector3 px_py = colliderB->GetVertices()[(i + 1) % vertexSize] - colliderB->GetVertices()[i];
		Vector3 m = pt_px.cross(px_py);
		// �ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
		if (m.dot(colliderB->GetNormal()) < -epsilon) { return false; }
	}

	// �����Ȃ̂œ������Ă���
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