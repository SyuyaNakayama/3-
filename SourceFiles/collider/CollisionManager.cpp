#include "CollisionManager.h"
#include <string>
#include <algorithm>
using namespace std;

std::list<Collider*> CollisionManager::colliders_;

bool CollisionManager::CheckBoxCollisionPair(Collider* colliderA, Collider* colliderB)
{
	if (!((UINT)colliderA->GetCollisionAttribute() & (UINT)colliderB->GetCollisionMask()) ||
		!((UINT)colliderB->GetCollisionAttribute() & (UINT)colliderA->GetCollisionMask()))
	{
		return false;
	}

	Vector3 vecAB = colliderA->GetWorldPosition() - colliderB->GetWorldPosition();
	Vector3 radAB = colliderA->GetRadius() + colliderB->GetRadius();
	vecAB = vecAB.abs();

	return vecAB <= radAB;
}

void CollisionManager::CheckAllCollisions()
{
	list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA)
	{
		Collider* colliderA = *itrA;
		list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB)
		{
			Collider* colliderB = *itrB;
			if (CheckBoxCollisionPair(colliderA, colliderB))
			{
				colliderA->OnCollision(colliderB);
				colliderB->OnCollision(colliderA);

				// �v���C���[�������ɓ��������瓖���蔻�菈���𔲂���
				if (colliderA->GetCollisionAttribute() == CollisionAttribute::Player ||
					colliderB->GetCollisionAttribute() == CollisionAttribute::Player)
				{
					return;
				}
			}
		}
		if (colliderA->GetCollisionAttribute() == CollisionAttribute::Player) { return; }
	}
}