#pragma once
#include "collider/Collider.h"
#include "WorldTransform.h"
#include "Model.h"
#include "TextureManager.h"
#include "input.h"
#include "ViewProjection.h"

class Player :public Collider
{
private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	Input* input_ = nullptr;
	uint32_t texture_ = 0;

	void Move();
public:
	void Initialize();
	void Update();
	void Draw();

	void OnCollision() {};
	const Vector3 GetWorldPosition() { return worldTransform_.translation_; }
	const Vector3 GetRadius() { return worldTransform_.scale_; }
};