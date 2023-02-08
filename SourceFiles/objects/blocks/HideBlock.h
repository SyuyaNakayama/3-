#pragma once
#include <Model.h>
#include <Input.h>

class HideBlock
{
private:
	Model* hideBlock = nullptr;
	WorldTransform worldTransform_;
	uint16_t *stageNum = 0;
	bool isHide = true;
	Vector3 rot;
	Input* input = nullptr;
public:
	void Initialize();
	void SetTransfer(Vector3 pos, Vector3 scale);
	void Update(uint16_t useCount,uint16_t setNum);
	void Draw();
};
