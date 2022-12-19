#pragma once
#include "Collider.h"
#include "Model.h"
#include "TextureManager.h"
#include "input.h"
#include "ViewProjection.h"

class Player :public Collider
{
private:
	Model* model_ = nullptr;
	float spdX = -0.1f;
	float epsilon = 1.0e-5; // “–‚½‚è”»’è‚Ì‚Æ‚«‚Ì”÷’²®‚Æ‚µ‚Äg‚¤

	void Move();
	Player() = default;
public:
	Player(const Player& obj) = delete;
	static Player* GetInstance();

	void Initialize();
	void Update();
	void Draw();

	void OnCollision(Collider* collider);
};