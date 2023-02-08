#pragma once
#include "Collider.h"
#include "Model.h"
#include "input.h"
#include "Jump.h"
#include <array>
#include <memory>

class Player : public BoxCollider, public IncludeCollider
{
private:
	enum class PartId
	{
		Body,	//��
		LegL,	//����
		LegR	//�E��
	};

	enum class Direction
	{
		Left, Right,
		Back // ��
	};

	std::array <std::unique_ptr<Model>, 3> models;
	WorldTransform parentWorldTransform_[3]{}; //�e�q�֌W�p��worldTransform
	Vector3 moveSpd = { -0.1f,0,0 };
	float epsilon = 1.0e-5f; // �����蔻��̂Ƃ��̔������Ƃ��Ďg��
	Jump jump;
	bool isFallCheck = false;
	bool isClimb = false;
	bool isLadderHit = false;
	Direction direction = Direction::Left;
	bool walkFlag = true;
	float walkPos = 0.0f;
	float speed = 0.03f; // ���̐U��̑���
	float oldPosX = 0.0f;
	bool isInitialize = false;
	Input* input = nullptr;

	void WalkMotion();	// �������[�V����
public:
	void Initialize();
	void Update();
	void Draw();
	void OnCollision(BoxCollider* boxCollider);
	void OnCollision(IncludeCollider* includeCollider);
};