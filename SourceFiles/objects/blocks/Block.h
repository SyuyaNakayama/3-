#pragma once
#include "Model.h"
#include "Collider.h"
#include "Input.h"
#include "Quaternion.h"

class BaseBlock
{
protected:
	Model* model = nullptr;
	uint32_t textureHandle = 0;
	int num_ = 0;
public:
	virtual ~BaseBlock() { delete model; }
	virtual void Initialize() = 0;
	virtual void Update() {};
	virtual void Draw() = 0;
	virtual bool IsDestroy() { return false; }
	virtual void SetTranslation(Vector3 translation) = 0;
	virtual void SetRotation(Vector3 rotation) {}
	virtual void SetScale(Vector3 scale) = 0;
	virtual std::unique_ptr<BaseBlock> NewBlockCreate() { return nullptr; }
	void SetTexture(const std::string& fileName);

	void SetNum(int num) { num_ = num; }
	int GetNum() { return num_; }
};

class BaseBlockCollider : public virtual BaseBlock, public BoxCollider
{
protected:
	Input* input = Input::GetInstance();

public:
	virtual void Initialize();
	~BaseBlockCollider() = default;
	void SetTranslation(Vector3 translation) { worldTransform.translation_ = translation; }
	void SetScale(Vector3 scale) { worldTransform.scale_ = scale; }
	void Draw();
};

class NonCollisionNormalBlock : public BaseBlock
{
private:
	WorldTransform worldTransform;

public:
	void Initialize();
	void SetTranslation(Vector3 translation) { worldTransform.translation_ = translation; }
	void SetScale(Vector3 scale) { worldTransform.scale_ = scale; }
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
	bool isCopy = true;
public:
	std::unique_ptr<BaseBlock> NewBlockCreate();
	void Initialize();
	void Update();
	void OnCollision(RayCollider* Collider);
};

class CopyedBlock : public BaseBlockCollider
{
public:
	void Initialize();
	void Update();
};

class DestroyBlock : public BaseBlockCollider, public PolygonCollider
{
private:
	uint32_t clickNum = 0;
	static const uint32_t DESTROY_NUM = 3;
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
	void SetRotation(Vector3 rotation) { worldTransform.rotation_ = rotation; }
};

class Button : public BaseBlockCollider
{
	bool isDraw = true;

private:
	bool isDestroy = false;
public:
	static uint16_t useCount;
public:
	bool IsDestroy() { return isDestroy; }
	void Initialize();
	void Draw() { if (isDraw) { model->Draw(worldTransform, *ViewProjection::GetInstance()); } }
	void OnCollision(BoxCollider* collider);
	void SetRotation(Vector3 rotation) { worldTransform.rotation_ = rotation; }
};

class GoalBlock : public BaseBlockCollider
{
public:
	void Initialize();
	void Draw();
};

class StagePlane : public PlaneCollider
{
	StagePlane() = default;
public:
	static StagePlane* GetInstance();
	void Initialize();
	void Rotation(Quaternion rotQ) { normal = Quaternion::RotateVector(normal, rotQ); }
};

class BgBlock : public BaseBlock
{
private:
	WorldTransform worldTransform;

public:
	void Initialize();
	void SetTranslation(Vector3 translation) { worldTransform.translation_ = translation; }
	void SetScale(Vector3 scale) { worldTransform.scale_ = scale; }
	void Draw() { model->Draw(worldTransform, *ViewProjection::GetInstance(), textureHandle); }
};

class StopBlock : public BaseBlockCollider
{
private:
	bool isDestroy = false;
public:
	bool IsDestroy() { return isDestroy; }
	void NumDestroy();
	void Initialize();
	void Update();
};