#pragma once
#include <Model.h>
#include <Input.h>

class HideBlock
{
private:
	Model* hideBlock = nullptr;
	WorldTransform worldTransform_;

	Input* input = nullptr;

	bool hideFlag = true;
public:
	void Initialize(Vector3 pos,Vector3 rot);
	void Update();
	void Draw();
};
