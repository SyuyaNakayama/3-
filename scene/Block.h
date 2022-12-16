#pragma once
#include "Model.h"
#include "collider/Collider.h"

class Block: Collider
{
private:
	Model* model;

public:
	void Initialize();
	void Update();
	void Draw();
};