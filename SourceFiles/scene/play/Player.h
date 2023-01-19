#pragma once
#include "Collider.h"
#include "Model.h"
#include "input.h"
#include "Jump.h"

class Player : public BoxCollider, public IncludeCollider
{
private:
	Model* model_ = nullptr;
	float spdX = -0.1f;
	float epsilon = 1.0e-5f; // 当たり判定のときの微調整として使う
	Jump jump;
	bool isFallCheck = false;
	bool isClimb = false;

	void Move();
public:
	~Player() { delete model_; }
	void Initialize();
	void Update();
	void Draw();

	void OnCollision(BoxCollider* boxCollider);
	void OnCollision(IncludeCollider* includeCollider);
};