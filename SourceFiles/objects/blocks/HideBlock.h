#pragma once
#include <Model.h>
#include <Input.h>

class HideBlock
{
private:
	Model* hideBlock = nullptr;
	WorldTransform worldTransform_;

	bool isHide = true;

	Input* input = nullptr;
public:
	void Initialize(Vector3 pos,Vector3 rot);
	void SetTransfer(Vector3 pos, Vector3 rot);
	bool SetFlag() { return isHide = true; }
	void Update(uint16_t useCount,uint16_t setNum);
	void Draw();
};
