#pragma once
#include "Model.h"
#include "Collider.h"

class BaseBlock : public BoxCollider
{
private:
	Model* model;
	uint32_t textureHandle = 0;

public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw();
	void SetTexture(const std::string& fileName);
	void SetTranslation(Vector3 translation) { worldTransform.translation_ = translation; }
	~BaseBlock() { delete model; }
};

class MoveBlock : public BaseBlock, TriangleCollider
{
private:
	bool isDrag = false;

	void DragBox();
public:
	void Initialize() override;
	void Update();
	void OnCollision(RayCollider* Collider) override;
};

class CopyBlock : public BaseBlock
{
public:
	void Initialize();
	void Update();

};

class DestroyBlock : public BaseBlock
{
private:
	uint32_t clickNum = 0;
	static const uint32_t DESTROY_NUM = 3;

	void Destroy();
public:
	void Initialize();
	void Update();
};