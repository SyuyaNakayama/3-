#pragma once
#include <Model.h>

class Skydome
{
private:
	Model* skydome = nullptr;
	WorldTransform worldTransform_;
public:
	void Initialize();
	void Update();
	void Draw();
};