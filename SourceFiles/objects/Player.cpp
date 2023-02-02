#include "Player.h"
#include "ImGuiManager.h"
#include "CollisionManager.h"
#include <imgui.h>
#include "Quaternion.h"

void Player::Initialize()
{
	//モデル読み込み
	models[(int)PartId::Body].reset(Model::CreateFromOBJ("player_body"));	// 体
	models[(int)PartId::LegL].reset(Model::CreateFromOBJ("player_legL"));	// 左足
	models[(int)PartId::LegR].reset(Model::CreateFromOBJ("player_legR"));	// 右足

	worldTransform.Initialize();
	worldTransform.scale_.x = 0.9999f;
	worldTransform.translation_ = { 36.0f ,-30.0f + epsilon,-39.0f };
	Quaternion rotQ = CubeQuaternion::Get(1);
	worldTransform.translation_ = Quaternion::RotateVector(worldTransform.translation_, rotQ);
	moveSpd= Quaternion::RotateVector(moveSpd, rotQ);

	//親子関係
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
	worldTransform.translation_ += moveSpd;
}

void Player::WalkMotion()
{
	const float X = 0.5f; //歩幅

	if (!walkFlag == true) { return; }
	if (fabsf(walkPos) >= X) { speed = -speed; }
	walkPos += speed;

	parentWorldTransform_[(int)PartId::LegL].translation_.z = walkPos;
	parentWorldTransform_[(int)PartId::LegR].translation_.z = -walkPos;
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
	case Player::Direction::Left:
		worldTransform.rotation_.y = PI / 2.0f;
		break;
	case Player::Direction::Right:
		worldTransform.rotation_.y = 1.5f * PI;
		break;
	case Player::Direction::Back:
		worldTransform.rotation_.y = PI;
		break;
	}

	//プレイヤーが向いている方向を求める
	if (oldPosX > worldTransform.translation_.x) { direction = Direction::Left; }
	else if (oldPosX < worldTransform.translation_.x) { direction = Direction::Right; }

	WalkMotion();
	worldTransform.Update();

	//現在の座標を保存する
	oldPosX = worldTransform.translation_.x;

	isClimb = isLadderHit = false;

	for (WorldTransform& w : parentWorldTransform_) { w.Update(); }
	ImGuiManager::GetInstance()->PrintVector("playerPos", worldTransform.GetWorldPosition());
}

void Player::Draw()
{
	for (size_t i = 0; i < models.size(); i++)
	{
		models[i]->Draw(parentWorldTransform_[i], *ViewProjection::GetInstance());
	}
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

	// ボタンの場合
	if (boxCollider->GetCollisionAttribute() == CollisionAttribute::Button) { return; }

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
	moveSpd = -moveSpd;
}

void Player::OnCollision(IncludeCollider* includeCollider)
{
	isClimb = true;
	if (isLadderHit)
	{
		worldTransform.translation_.y += 0.01f;
		direction = Direction::Back;
	}
}