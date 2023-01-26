#pragma once
#include "Model.h"
#include "Collider.h"
#include "Input.h"

class BaseBlock
{
protected:
	Model* model = nullptr;
	uint32_t textureHandle = 0;

public:
	virtual ~BaseBlock() { delete model; }
	virtual void Initialize() = 0;
	virtual void Update() {};
	virtual void Draw() = 0;
	virtual bool IsDestroy() { return false; }
	virtual void SetTranslation(Vector3 translation) = 0;
virtual void SetRotation(Vector3 rotation){}
	virtual std::unique_ptr<BaseBlock> NewBlockCreate() { return nullptr; }
	void SetTexture(const std::string& fileName);
};

class BaseBlockCollider : public virtual BaseBlock, public BoxCollider
{
protected:
	Input* input = Input::GetInstance();

public:
	virtual void Initialize();
	~BaseBlockCollider() = default;
	void SetTranslation(Vector3 translation) { worldTransform.translation_ = translation; }
	void SetRotation(Vector3 rotation){worldTransform.rotation_ = rotation_;}
void Draw();
};

class NonCollisionNormalBlock : public BaseBlock
{
private:
	WorldTransform worldTransform;

public:
	void Initialize();
	void SetTranslation(Vector3 translation) { worldTransform.translation_ = translation; }
	void Draw() { model->Draw(worldTransform, *ViewProjection::GetInstance(), textureHandle); }
};

class NormalBlock : public BaseBlockCollider
{
	void Initialize();
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

class CopyBlock : public BaseBlockCollider, public PolygonCollider
{
private:
	bool isCopyMode = false;
public:
	std::unique_ptr<BaseBlock> NewBlockCreate();
	void Initialize();
	void Update();
	void OnCollision(RayCollider* Collider);
};

class DestroyBlock : public BaseBlockCollider, public PolygonCollider
{
private:
	uint32_t clickNum = 0;
	static const uint32_t DESTROY_NUM = 2;
	bool isDestroy = false;

public:
	void Initialize();
	void Update();
	bool IsDestroy() { return isDestroy; }
	void OnCollision(RayCollider* collider) override;
};

class LadderBlock : public BaseBlockCollider, public IncludeCollider
{
public:
	void Initialize();
	void Draw() { model->Draw(worldTransform, *ViewProjection::GetInstance()); }
	Vector3 GetWorldPosition() { return worldTransform.GetWorldPosition() + Vector3(0, 0, 1); }
};

class Button : public BaseBlockCollider
{
	bool isDraw = true;
public:
	void Initialize();
	void Draw() { if (isDraw) { model->Draw(worldTransform, *ViewProjection::GetInstance()); } }
	void OnCollision(BoxCollider* collider);
};

class StagePlane : public PlaneCollider
{
	StagePlane() = default;
public:
	static StagePlane* GetInstance();
	void Initialize();
};

class BgBlock : public BaseBlock
{
private:
	WorldTransform worldTransform;

public:
	void Initialize();
	void SetTranslation(Vector3 translation) { worldTransform.translation_ = translation; }
	void Draw() { model->Draw(worldTransform, *ViewProjection::GetInstance(), textureHandle); }
};
