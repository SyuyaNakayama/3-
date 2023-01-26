#include "Player.h"
#include "ImGuiManager.h"
#include "CollisionManager.h"
#include <imgui.h>

void Player::Initialize()
{
	//モデル読み込み
	modelBody_ = Model::CreateFromOBJ("player_body");	// 体
	//modelBody_ = Model::CreateFromOBJ("lever");	// 体
	modelLegL_ = Model::CreateFromOBJ("player_legL");	// 左足
	modelLegR_ = Model::CreateFromOBJ("player_legR");	// 右足

	worldTransform.Initialize();
	worldTransform.scale_.x = 0.9999f;
	worldTransform.translation_ = { 76.0f ,-76.0f + epsilon,0 };

	//親子関係
	//体
	for (WorldTransform& w : parentWorldTransform_)
	{
		w.Initialize();
		w.parent_ = &worldTransform;
		w.translation_ = { 0.0f,0.9f,0.0f };
	}

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
	const float X = 0.5f; //歩幅

	if (!walkFlag == true) { return; }
	if (fabsf(walkPos) >= X) { speed = -speed; }
	walkPos += speed;

	ImGui::Text("X:%f", walkPos);
	parentWorldTransform_[PartId::kLegL].translation_.z = walkPos;
	parentWorldTransform_[PartId::kLegR].translation_.z = -walkPos;
}

void Player::Update()
{
	UINT16 flagSum = isClimb + isLadderHit;
	switch (flagSum)
	{
	case 0:
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
		if (jump.IsFall()) { break; }
		jump.UpdateJump(worldTransform.translation_.y);
	case 1:	Move();	break;
	}
	//進んでる方向によってキャラの向きを変える
	switch (direction)
	{
	case 0://左
		worldTransform.rotation_.y = 90 * PI / 180;
		break;
	case 1://右
		worldTransform.rotation_.y = 270 * PI / 180;
		break;
	case 2://奥
		worldTransform.rotation_.y = 180 * PI / 180;
		break;
	}

	//プレイヤーが向いている方向を求める
	if (oldPos > worldTransform.translation_.x) { direction = 0; }
	else if (oldPos < worldTransform.translation_.x) { direction = 1; }

	WalkMotion();
	worldTransform.Update();

	//現在の座標を保存する
	oldPos = worldTransform.translation_.x;

	isClimb = isLadderHit = false;

	for (WorldTransform& w : parentWorldTransform_) { w.Update(); }
}

void Player::Draw()
{
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

	if (boxCollider->GetCollisionAttribute() == CollisionAttribute::LadderBlock)
	{
		// 梯子ブロックの場合
		isLadderHit = true;
		return;
	}

	if (boxCollider->GetCollisionAttribute() == CollisionAttribute::Button)
	{
		// ボタンの場合
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

	float playerTopPosition = BoxCollider::GetWorldPosition().y + GetRadius().y;

	if (blockTopPosition - playerTopPosition <= epsilon)
	{
		// 高さ1マスならジャンプ
		jump.StartJump(0.5f, blockTopPosition + epsilon + GetRadius().y);
		return;
	}

	// それ以外なら撥ね返る
	spdX = -spdX;
}

void Player::OnCollision(IncludeCollider* includeCollider)
{
	isClimb = true;
	if (isLadderHit) {
		worldTransform.translation_.y += 0.01f;
		direction = 2;
	}
}