#pragma once
#include <Model.h>


class HideBlock
{
private:
	Model* hideBlock = nullptr;
	WorldTransform worldTransform_;

	Input* input = nullptr;//Input::GetInstance();
public:
	void Initialize();
	void Update();
	void Draw();
};
