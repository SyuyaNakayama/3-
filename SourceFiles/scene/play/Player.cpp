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
	// 落下チェック(ジャンプ中でないとき)
	if (!jump.IsJump())
	{
		worldTransform.translation_.y -= epsilon;
		worldTransform.Update();
		isFallCheck = true;
		CollisionManager collisionManager;
		collisionManager.CheckAllCollisions();
		worldTransform.translation_.y += epsilon;

		// 下に地面がなかったら落下
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
		// 落下チェックのときは終了
		isFallCheck = false;
		return;
	}

	float blockTopPosition = collider->GetWorldPosition().y + collider->GetRadius().y;
	if (jump.IsFall())
	{
		// 落下中に地面に当たったら落下終了
		jump.EndFall();
		worldTransform.translation_.y = blockTopPosition + epsilon + GetRadius().y;
		return;
	}

	float playerTopPosition = GetWorldPosition().y + GetRadius().y;

	if (blockTopPosition - playerTopPosition <= epsilon)
	{
		// 高さ1マスならジャンプ
		jump.StartJump(0.5f, blockTopPosition + epsilon + GetRadius().y);
		return;
	}
	// それ以外なら撥ね返る
	spdX = -spdX;
}