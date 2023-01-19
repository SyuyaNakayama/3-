#pragma once
#include "Collider.h"
#include "Model.h"
#include "input.h"
#include "Jump.h"

class Player : public BoxCollider
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

	void Move();
public:
	~Player() { delete model_; }
	void Initialize();
	void Update();
	void Draw();

	void OnCollision(BoxCollider* boxCollider);
};