#include "Player.h"
#include "ImGuiManager.h"
#include "CollisionManager.h"
#include <imgui.h>

void Player::Initialize()
{
	//���f���ǂݍ���
	modelBody_ = Model::CreateFromOBJ("player_body");	// ��
	//modelBody_ = Model::CreateFromOBJ("lever");	// ��
	modelLegL_ = Model::CreateFromOBJ("player_legL");	// ����
	modelLegR_ = Model::CreateFromOBJ("player_legR");	// �E��

	worldTransform.Initialize();
	worldTransform.scale_.x = 0.9999f;
	worldTransform.translation_ = { 76.0f ,-76.0f + epsilon,0 };

	//�e�q�֌W
	//��
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
	const float X = 0.5f; //����

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
	//�i��ł�����ɂ���ăL�����̌�����ς���
	switch (direction)
	{
	case 0://��
		worldTransform.rotation_.y = 90 * PI / 180;
		break;
	case 1://�E
		worldTransform.rotation_.y = 270 * PI / 180;
		break;
	case 2://��
		worldTransform.rotation_.y = 180 * PI / 180;
		break;
	}

	//�v���C���[�������Ă�����������߂�
	if (oldPos > worldTransform.translation_.x) { direction = 0; }
	else if (oldPos < worldTransform.translation_.x) { direction = 1; }

	WalkMotion();
	worldTransform.Update();

	//���݂̍��W��ۑ�����
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

	if (boxCollider->GetCollisionAttribute() == CollisionAttribute::Button)
	{
		// �{�^���̏ꍇ
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
	if (isLadderHit) {
		worldTransform.translation_.y += 0.01f;
		direction = 2;
	}
}