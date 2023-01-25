#pragma once
#include "Collider.h"
#include "Model.h"
#include "input.h"
#include "Jump.h"

class Player : public BoxCollider, public IncludeCollider
{
private:
	Model* model_ = nullptr;
	Model* modelBody_ = nullptr;	//�̂̃��f��
	Model* modelLegL_ = nullptr;	//�����̃��f��
	Model* modelLegR_ = nullptr;	//�E���̃��f��
	float spdX = -0.1f;
	float epsilon = 1.0e-5f; // �����蔻��̂Ƃ��̔������Ƃ��Ďg��
	Jump jump;
	bool isFallCheck = false;
	bool isClimb = false;
	bool isLadderHit = false;
	WorldTransform parentWorldTransform_[3] = {}; //�e�q�֌W�p��worldTransform

	uint16_t direction = 0;	//�L�����̕���(0...��, 1...�E, 2...��)
	bool walkFlag = true;
	float walkPos = 0.0f;
	bool ForB = true; //true...�O, false...��
	float oldPos = 0.0f;

	Input* input = nullptr;

	void Move();
	void WalkMotion();	//�������[�V����
public:
	~Player() { delete model_; }
	void Initialize();
	void Update();
	void Draw();

	void OnCollision(BoxCollider* boxCollider);
	void OnCollision(IncludeCollider* includeCollider);

	enum PartId {
		kBody,	//��
		kLegL,	//����
		kLegR	//�E��
	};
};