#include "Player.h"
#include "ImGuiManager.h"
#include "CollisionManager.h"

Player* Player::GetInstance()
{
	static Player instance;
	return &instance;
}

void Player::Initialize()
{
	model_ = Model::Create();
	worldTransform.Initialize();
	worldTransform.scale_.x = 0.9999f;
	worldTransform.translation_ = { 20.0f ,2.0f + epsilon,0 };
	jump.SetGravity(0.08f);
}

void Player::Move()
{
	worldTransform.translation_.x += spdX;
}

void Player::Update()
{
	// �����`�F�b�N(�W�����v���łȂ��Ƃ�)
	if (!jump.IsJump())
	{
		worldTransform.translation_.y -= epsilon;
		worldTransform.Update();
		isFallCheck = true;
		CollisionManager collisionManager;
		collisionManager.CheckAllCollisions();
		worldTransform.translation_.y += epsilon;

		// ���ɒn�ʂ��Ȃ������痎��
		if (isFallCheck)
		{
			jump.StartFall();
			isFallCheck = false;
			jump.UpdateFall(worldTransform.translation_.y);
		}
	}
	if (!jump.IsFall())
	{
		Move();
		jump.UpdateJump(worldTransform.translation_.y);
	}
	worldTransform.Update();
}

void Player::Draw()
{
	model_->Draw(worldTransform, *ViewProjection::GetInstance());
}

void Player::OnCollision(Collider* collider)
{
	if (isFallCheck)
	{
		// �����`�F�b�N�̂Ƃ��͏I��
		isFallCheck = false;
		return;
	}

	float blockTopPosition = collider->GetWorldPosition().y + collider->GetRadius().y;
	if (jump.IsFall())
	{
		// �������ɒn�ʂɓ��������痎���I��
		jump.EndFall();
		worldTransform.translation_.y = blockTopPosition + epsilon + GetRadius().y;
		return;
	}

	float playerTopPosition = GetWorldPosition().y + GetRadius().y;

	if (blockTopPosition - playerTopPosition <= epsilon)
	{
		// ����1�}�X�Ȃ�W�����v
		jump.StartJump(0.5f, blockTopPosition + epsilon + GetRadius().y);
		return;
	}
	// ����ȊO�Ȃ睛�˕Ԃ�
	spdX = -spdX;
}