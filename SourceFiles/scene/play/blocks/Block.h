#pragma once
#include "Model.h"
#include "Collider.h"
#include "Input.h"

class BaseBlock
{
protected:
	Model* model;
	uint32_t textureHandle = 0;

public:
	~BaseBlock() { delete model; }
	virtual void Initialize() = 0;
	virtual void Update() {};
	virtual void Draw() = 0;
	void SetTexture(const std::string& fileName);
	virtual void SetTranslation(Vector3 translation) = 0;
};

class BaseBlockCollider : public virtual BaseBlock, public BoxCollider
{
protected:
	Input* input = Input::GetInstance();

public:
	virtual void Initialize();
	~BaseBlockCollider() = default;
	void SetTranslation(Vector3 translation) { worldTransform.translation_ = translation; }
	void Draw();
};

class NonCollisionNormalBlock : public BaseBlock
{
private:
	WorldTransform worldTransform;

public:
	void Initialize();
	virtual void SetTranslation(Vector3 translation) { worldTransform.translation_ = translation; }
};

class NormalBlock : public BaseBlockCollider
{
	void Initialize();
	void OnCollision(BoxCollider* Collider);
};

class MoveBlock : public BaseBlockCollider, public PolygonCollider
{
private:
	bool isDrag = false;

	void DragBox();
public:
	void Initialize();
	void Update();
	void OnCollision(RayCollider* Collider);
};

class CopyBlock : public BaseBlockCollider
{
public:
	void Initialize();
	void Update();
};

class DestroyBlock : public BaseBlockCollider
{
private:
	uint32_t clickNum = 0;
	static const uint32_t DESTROY_NUM = 3;

	void Destroy();
public:
	void Initialize();
	void Update();
};

class StagePlane : public PlaneCollider
{
	StagePlane() = default;
public:
	static StagePlane* GetInstance();
	void Initialize();
	void OnCollision(RayCollider* Collider);
};