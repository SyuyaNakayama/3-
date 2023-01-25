#pragma once
#include "Collider.h"
#include "Model.h"
#include "input.h"
#include "Jump.h"

class Player : public BoxCollider, public IncludeCollider
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
	bool isClimb = false;
	bool isLadderHit = false;
	WorldTransform parentWorldTransform_[3] = {}; //親子関係用のworldTransform

	uint16_t direction = 0;	//キャラの方向(0...左, 1...右, 2...奥)
	bool walkFlag = true;
	float walkPos = 0.0f;
	bool ForB = true; //true...前, false...後
	float oldPos = 0.0f;

	Input* input = nullptr;

	void Move();
	void WalkMotion();	//歩くモーション
public:
	~Player() { delete model_; }
	void Initialize();
	void Update();
	void Draw();

	void OnCollision(BoxCollider* boxCollider);
	void OnCollision(IncludeCollider* includeCollider);

	enum PartId {
		kBody,	//体
		kLegL,	//左足
		kLegR	//右足
	};
};