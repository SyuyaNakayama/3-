#pragma once
#include "Collider.h"
#include "Model.h"
#include "input.h"
#include "Jump.h"

class Player : public BoxCollider
{
private:
	Model* model_ = nullptr;
	Model* modelBody_ = nullptr;	//体のモデル
	Model* modelLegL_ = nullptr;	//左足のモデル
	Model* modelLegR_ = nullptr;	//右足のモデル
	float spdX = -0.1f;
	float epsilon = 1.0e-5f; // 当たり判定のときの微調整として使う
	Jump jump;
	bool isFallCheck = false;
	WorldTransform parentWorldTransform_[3] = {}; //親子関係用のworldTransform

	void Move();
public:
	~Player() { delete model_; }
	void Initialize();
	void Update();
	void Draw();

	void OnCollision(BoxCollider* boxCollider);

	enum PartId {
		kBody,	//体
		kLegL,	//左足
		kLegR	//右足
	};
};