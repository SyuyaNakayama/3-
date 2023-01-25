#include "Player.h"
#include "ImGuiManager.h"
#include "CollisionManager.h"
#include <imgui.h>

void Player::Initialize()
{
	//���f���ǂݍ���
	model_ = Model::Create();							// �匳
	modelBody_ = Model::CreateFromOBJ("player_body");	// ��
	modelLegL_ = Model::CreateFromOBJ("player_legL");	// ����
	modelLegR_ = Model::CreateFromOBJ("player_legR");	// �E��

	worldTransform.Initialize();
	worldTransform.scale_.x = 0.9999f;
	worldTransform.translation_ = { 76.0f ,-76.0f + epsilon,0 };

	//�e�q�֌W
	//��
	parentWorldTransform_[PartId::kBody].Initialize();
	parentWorldTransform_[PartId::kBody].parent_ = &worldTransform;
	parentWorldTransform_[PartId::kBody].translation_ = { 0.0f,0.9f,0.0f };
	//����
	parentWorldTransform_[PartId::kLegL].Initialize();
	parentWorldTransform_[PartId::kLegL].parent_ = &worldTransform;
	parentWorldTransform_[PartId::kLegL].translation_ = { 0.0f,0.9f,0.0f };
	//�E��
	parentWorldTransform_[PartId::kLegR].Initialize();
	parentWorldTransform_[PartId::kLegR].parent_ = &worldTransform;
	parentWorldTransform_[PartId::kLegR].translation_ = { 0.0f,0.9f,0.0f };
	
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
	float X = 0.5f;		//����
	float speed = 0.03f;//���̐U��̑���

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

	WalkMotion();

	worldTransform.Update();
	isClimb = isLadderHit = false;

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

	if (direction == 0) { direction = 1; }
	else if (direction == 1) { direction = 0; }
}

void Player::OnCollision(IncludeCollider* includeCollider)
{
	isClimb = true;
	if (isLadderHit) { worldTransform.translation_.y += 0.01f; }
}