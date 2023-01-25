#include "Player.h"
#include "ImGuiManager.h"
#include "CollisionManager.h"
#include <imgui.h>

void Player::Initialize()
{
	model_ = Model::Create();
	worldTransform.Initialize();
	worldTransform.scale_.x = 0.9999f;
	worldTransform.translation_ = { 76.0f ,-76.0f + epsilon,0 };
	jump.SetGravity(0.08f);
	SetCollisionAttribute(CollisionAttribute::Player);
	SetCollisionMask(CollisionMask::Player);
}

void Player::Move() { worldTransform.translation_.x += spdX; }

void Player::Update()
{
	UINT16 flagSum = isClimb + isLadderHit;
	switch (flagSum)
	{
	case 0:
		// �����`�F�b�N(�W�����v���łȂ��Ƃ�)
		if (!jump.IsJump())
		{
			worldTransform.translation_.y -= epsilon;
			worldTransform.Update();
			isFallCheck = true;
			CollisionManager::CheckBoxCollisions();
			worldTransform.translation_.y += epsilon;

			// ���ɒn�ʂ��Ȃ������痎��
			if (isFallCheck)
			{
				jump.StartFall();
				isFallCheck = false;
				jump.UpdateFall(worldTransform.translation_.y);
			}
		}
		if (jump.IsFall()) { break; }
		jump.UpdateJump(worldTransform.translation_.y);
	case 1:	Move();	break;
	}

	worldTransform.Update();
	isClimb = isLadderHit = false;
}

void Player::Draw() { model_->Draw(worldTransform, *ViewProjection::GetInstance()); }

void Player::OnCollision(BoxCollider* boxCollider)
{
	if (isFallCheck)
	{
		// �����`�F�b�N�̂Ƃ��͏I��
		isFallCheck = false;
		return;
	}

	if (boxCollider->GetCollisionAttribute() == CollisionAttribute::LadderBlock)
	{
		isLadderHit = true;
		return;
	}

	float blockTopPosition = boxCollider->GetWorldPosition().y + boxCollider->GetRadius().y;
	if (jump.IsFall())
	{
		// �������ɒn�ʂɓ��������痎���I��
		jump.EndFall();
		worldTransform.translation_.y = blockTopPosition + epsilon + GetRadius().y;
		return;
	}

	float playerTopPosition = BoxCollider::GetWorldPosition().y + GetRadius().y;

	if (blockTopPosition - playerTopPosition <= epsilon)
	{
		// ����1�}�X�Ȃ�W�����v
		jump.StartJump(0.5f, blockTopPosition + epsilon + GetRadius().y);
		return;
	}

	// ����ȊO�Ȃ睛�˕Ԃ�
	spdX = -spdX;
}

void Player::OnCollision(IncludeCollider* includeCollider)
{
	isClimb = true;
	if (isLadderHit) { worldTransform.translation_.y += 0.01f; }
}