#include "Player.h"
#include "CollisionManager.h"
#include "Quaternion.h"
#include "GameScene.h"

void Player::Initialize()
{
	Quaternion rotQ = CubeQuaternion::Get();
	worldTransform.translation_ = RotateVector({ 36.0f ,-35.0f + epsilon,-39.0f }, rotQ);
	moveSpd = RotateVector(moveSpd, rotQ);
	direction = Direction::Left;

	if (isInitialize) { return; }
	//モデル読み込み
	models[(int)PartId::Body].reset(Model::CreateFromOBJ("player_body"));	// 体
	models[(int)PartId::LegL].reset(Model::CreateFromOBJ("player_legL"));	// 左足
	models[(int)PartId::LegR].reset(Model::CreateFromOBJ("player_legR"));	// 右足

	worldTransform.Initialize();
	worldTransform.scale_.x = 0.9999f;

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

	isInitialize = true;
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
	case 1:	worldTransform.translation_ += moveSpd;	break;
	}

	if (!isClimb)
	{
		// プレイヤーが向いている方向を求める
		Vector3 dVec = { -1,0,0 }; // 向き判別に使うベクトル
		dVec = RotateVector(dVec, CubeQuaternion::Get());
		float dot = Vector3Dot(dVec, moveSpd); // 向き判別ベクトルとプレイヤー移動速度ベクトルの内積を取る
		if (dot < 0.0f) { direction = Direction::Right; }
		if (dot > 0.0f) { direction = Direction::Left; }
	}

	// 進んでる方向によってキャラの向きを変える
	std::array<float, 3> r = { PI / 2.0f,1.5f * PI,PI };
	worldTransform.rotation_.y = r[(size_t)direction] - PI / 2.0f * (*GameScene::GetStage() - 1);

	WalkMotion();

	// 現在の座標を保存する
	oldPosX = worldTransform.translation_.x;

	isClimb = isLadderHit = false;

	worldTransform.Update();
	for (WorldTransform& w : parentWorldTransform_) { w.Update(); }
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

	if (blockTopPosition - playerTopPosition <= epsilon + 0.3f)
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