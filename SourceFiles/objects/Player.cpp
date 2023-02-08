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
	//���f���ǂݍ���
	models[(int)PartId::Body].reset(Model::CreateFromOBJ("player_body"));	// ��
	models[(int)PartId::LegL].reset(Model::CreateFromOBJ("player_legL"));	// ����
	models[(int)PartId::LegR].reset(Model::CreateFromOBJ("player_legR"));	// �E��

	worldTransform.Initialize();
	worldTransform.scale_.x = 0.9999f;

	//�e�q�֌W
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
	const float X = 0.5f; //����

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
	case 1:	worldTransform.translation_ += moveSpd;	break;
	}

	if (!isClimb)
	{
		// �v���C���[�������Ă�����������߂�
		Vector3 dVec = { -1,0,0 }; // �������ʂɎg���x�N�g��
		dVec = RotateVector(dVec, CubeQuaternion::Get());
		float dot = Vector3Dot(dVec, moveSpd); // �������ʃx�N�g���ƃv���C���[�ړ����x�x�N�g���̓��ς����
		if (dot < 0.0f) { direction = Direction::Right; }
		if (dot > 0.0f) { direction = Direction::Left; }
	}

	// �i��ł�����ɂ���ăL�����̌�����ς���
	std::array<float, 3> r = { PI / 2.0f,1.5f * PI,PI };
	worldTransform.rotation_.y = r[(size_t)direction] - PI / 2.0f * (*GameScene::GetStage() - 1);

	WalkMotion();

	// ���݂̍��W��ۑ�����
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
		// �����`�F�b�N�̂Ƃ��͏I��
		isFallCheck = false;
		return;
	}

	if (boxCollider->GetCollisionAttribute() == CollisionAttribute::LadderBlock)
	{
		// ��q�u���b�N�̏ꍇ
		isLadderHit = true;
		return;
	}

	// �{�^���̏ꍇ
	if (boxCollider->GetCollisionAttribute() == CollisionAttribute::Button) { return; }

	float blockTopPosition = boxCollider->GetWorldPosition().y + boxCollider->GetRadius().y;
	if (jump.IsFall())
	{
		// �������ɒn�ʂɓ��������痎���I��
		jump.EndFall();
		worldTransform.translation_.y = blockTopPosition + epsilon + GetRadius().y;
		return;
	}

	float playerTopPosition = BoxCollider::GetWorldPosition().y + GetRadius().y;

	if (blockTopPosition - playerTopPosition <= epsilon + 0.3f)
	{
		// ����1�}�X�Ȃ�W�����v
		jump.StartJump(0.5f, blockTopPosition + epsilon + GetRadius().y);
		return;
	}

	// ����ȊO�Ȃ睛�˕Ԃ�
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