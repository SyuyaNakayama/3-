#pragma once
#include "Model.h"
#include "Collider.h"

class Block: public Collider
{
private:
	Model* model;
	bool isDrag = false;

	void DragBox();
public:
	void Initialize();
	void Update();
	void Draw();
	void SetScale(Vector3 scale) { worldTransform.scale_ = scale; }
	void SetTranslation(Vector3 translation) { worldTransform.translation_ = translation; }
};