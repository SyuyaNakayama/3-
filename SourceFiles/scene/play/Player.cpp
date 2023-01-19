#include "Player.h"
#include "ImGuiManager.h"
#include "CollisionManager.h"
#include <imgui.h>

void Player::Initialize()
{
	//モデル読み込み
	model_ = Model::Create();							// 大元
	modelBody_ = Model::CreateFromOBJ("player_body");	// 体
	modelLegL_ = Model::CreateFromOBJ("player_legL");	// 左足
	modelLegR_ = Model::CreateFromOBJ("player_legR");	// 右足

	worldTransform.Initialize();
	worldTransform.scale_.x = 0.9999f;
	worldTransform.translation_ = { 76.0f ,-76.0f + epsilon,0 };

	//親子関係
	//体
	parentWorldTransform_[PartId::kBody].Initialize();
	parentWorldTransform_[PartId::kBody].parent_ = &worldTransform;
	parentWorldTransform_[PartId::kBody].translation_ = { 0.0f,0.9f,0.0f };
	//parentWorldTransform_[0].scale_ = {10.0f,10.0f,10.0f};
	//左足
	parentWorldTransform_[PartId::kLegL].Initialize();
	parentWorldTransform_[PartId::kLegL].parent_ = &worldTransform;
	parentWorldTransform_[PartId::kLegL].translation_ = { 0.0f,0.9f,0.0f };
	//parentWorldTransform_[PartId::kLegL].rotation_.y = 90 * PI / 180;
	//parentWorldTransform_[1].scale_ = { 10.0f,10.0f,10.0f };
	//右足
	parentWorldTransform_[PartId::kLegR].Initialize();
	parentWorldTransform_[PartId::kLegR].parent_ = &worldTransform;
	parentWorldTransform_[PartId::kLegR].translation_ = { 0.0f,0.9f,0.0f };
	//parentWorldTransform_[PartId::kLegR].rotation_.y = 90 * PI / 180;
	//parentWorldTransform_[2].scale_ = { 10.0f,10.0f,10.0f };

	jump.SetGravity(0.08f);
	SetCollisionAttribute(CollisionAttribute::Player);
	SetCollisionMask(CollisionMask::Player);
}

void Player::Move()
{
	worldTransform.translation_.x += spdX;
}

void Player::WalkMotion()
{
	float X = 0.5f;		//歩幅
	float speed = 0.03f;//足の振りの速さ

	if(walkFlag == true)
	{
		if(ForB == true)
		{
			walkPos += speed;
			if(walkPos >= X)
			{
				ForB = false;
			}
		}
		if(ForB == false)
		{
			walkPos -= speed;
			if (walkPos <= -X)
			{
				ForB = true;
			}
		}
		ImGui::Text("X:%f", walkPos);
		parentWorldTransform_[PartId::kLegL].translation_.z = walkPos;
		parentWorldTransform_[PartId::kLegR].translation_.z = -walkPos;
	}
}

void Player::Update()
{
	// 落下チェック(ジャンプ中でないとき)
	if (!jump.IsJump())
	{
		worldTransform.translation_.y -= epsilon;
		worldTransform.Update();
		isFallCheck = true;
		CollisionManager::CheckBoxCollisions();
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
	//進んでる方向によってキャラの向きを変える
	if(direction == 0)
	{
		worldTransform.rotation_.y = 90 * PI / 180;
	}
	else if(direction == 1)
	{
		worldTransform.rotation_.y = 270 * PI / 180;
	}

	WalkMotion();

	worldTransform.Update();

	parentWorldTransform_[PartId::kBody].Update();
	parentWorldTransform_[PartId::kLegL].Update();
	parentWorldTransform_[PartId::kLegR].Update();

}

void Player::Draw()
{
	//model_->Draw(worldTransform, *ViewProjection::GetInstance());
	modelBody_->Draw(parentWorldTransform_[PartId::kBody], *ViewProjection::GetInstance());
	modelLegL_->Draw(parentWorldTransform_[PartId::kLegL], *ViewProjection::GetInstance());
	modelLegR_->Draw(parentWorldTransform_[PartId::kLegR], *ViewProjection::GetInstance());
}

void Player::OnCollision(BoxCollider* boxCollider)
{
	if (isFallCheck)
	{
		// 落下チェックのときは終了
		isFallCheck = false;
		return;
	}

	float blockTopPosition = boxCollider->GetWorldPosition().y + boxCollider->GetRadius().y;
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

	if (direction == 0) { direction = 1; }
	else if (direction == 1) { direction = 0; }
}