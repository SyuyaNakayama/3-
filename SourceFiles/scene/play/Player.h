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

	uint16_t direction = 0;	//キャラの方向(0...左,1...右)
	float walkTimer = 0;
	bool walkFlag = true;
	float walkPos = 0.0f;
	bool ForB = true; //true...前, false...後

	void Move();
	void WalkMotion();	//歩くモーション
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