#pragma once
#include "Model.h"
#include "collider/Collider.h"

class Block: Collider
{
private:
	Model* model;
	bool isDrag = false;

public:
	void Initialize();
	void Update();
	void Draw();
	void SetScale(Vector3 scale) { worldTransform.scale_ = scale; }
	void SetTranslation(Vector3 translation) { worldTransform.translation_ = translation; }
};