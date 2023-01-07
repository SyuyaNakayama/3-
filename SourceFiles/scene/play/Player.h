#pragma once
#include "Collider.h"
#include "Model.h"
#include "input.h"
#include "Jump.h"

class Player : public Collider
{
private:
	Model* model_ = nullptr;
	float spdX = -0.1f;
	float epsilon = 1.0e-5f; // “–‚½‚è”»’è‚Ì‚Æ‚«‚Ì”÷’²®‚Æ‚µ‚Äg‚¤
	Jump jump;
	bool isFallCheck = false;

	void Move();
public:
	~Player() { delete model_; }
	void Initialize();
	void Update();
	void Draw();

	void OnCollision(Collider* collider);
};