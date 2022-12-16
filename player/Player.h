#pragma once
#include "collider/Collider.h"
#include "Model.h"
#include "TextureManager.h"
#include "input.h"
#include "ViewProjection.h"

class Player :public Collider
{
private:
	Model* model_ = nullptr;

	void Move();
	Player() = default;
public:
	Player(const Player& obj) = delete;
	static Player* GetInstance();

	void Initialize();
	void Update();
	void Draw();

	void OnCollision() {};
};