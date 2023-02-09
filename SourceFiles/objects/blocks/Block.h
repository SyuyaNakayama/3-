#pragma once
#include "Model.h"
#include "Collider.h"
#include "Input.h"
#include "Sprite.h"
#include "Quaternion.h"

class BaseBlock
{
protected:
	Model* model = nullptr;
	uint32_t textureHandle = 0;
	static UINT16* nowStage;
public:
	virtual ~BaseBlock() { delete model; }
	virtual void Initialize();
	virtual void Update() {};
	virtual void Draw() = 0;
	virtual bool IsDestroy() { return false; }
	virtual void SetTranslation(Vector3 translation) = 0;
	virtual void SetRotation(Vector3 rotation) {}
	virtual void SetScale(Vector3 scale) {}
	virtual std::unique_ptr<BaseBlock> NewBlockCreate() { return nullptr; }
	virtual void SpriteDraw() {}
	virtual void SetNum(int num_) {}
	void SetTexture(const std::string& fileName) { textureHandle = TextureManager::Load("blockTextures/" + fileName); }
};

class BaseBlockCollider : public virtual BaseBlock, public BoxCollider
{
protected:
	Input* input = Input::GetInstance();
	int num_ = 0;

public:
	virtual void Initialize();
	~BaseBlockCollider() = default;
	void SetTranslation(Vector3 translation) { worldTransform.translation_ = translation; }
	void SetScale(Vector3 scale) { worldTransform.scale_ = scale; }
	void Draw(); //{ model->Draw(worldTransform, *ViewProjection::GetInstance(), textureHandle); }
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
	static bool isExplanation;
	void Initialize();
	void Update();
	void OnCollision(RayCollider* Collider) { if (Input::GetInstance()->IsTriggerMouse(0)) { isDrag = true; } }
};

class CopyBlock : public BaseBlockCollider, public PolygonCollider
{
private:
	bool isCopyMode = false;
	bool isCopy = true;
	Sprite* copyBlockShadow = nullptr;
public:
	static bool isExplanation;
	std::unique_ptr<BaseBlock> NewBlockCreate();
	void Initialize();
	void Update();
	void SpriteDraw() { if (isCopyMode) { copyBlockShadow->Draw(); } }
	void OnCollision(RayCollider* Collider) { if (input->IsTriggerMouse(0)) { isCopyMode = true; } }
};

class CopyedBlock : public BaseBlockCollider
{
public:
	void Initialize();
	void Update() { worldTransform.Update(); }
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
private:
	bool isDestroy = false;
	static uint16_t useCount;

public:
	bool IsDestroy() { return isDestroy; }
	void Initialize();
	void Draw() { model->Draw(worldTransform, *ViewProjection::GetInstance()); }
	void OnCollision(BoxCollider* collider);
	void SetRotation(Vector3 rotation) { worldTransform.rotation_ = rotation; }
	static void SetUseCount(UINT16 useCount_) { useCount = useCount_; }
	static UINT16 GetUseCount() { return useCount; }
};

class GoalBlock : public BaseBlock, public IncludeCollider
{
	static bool isGoal;
public:
	void Initialize();
	void Draw() { model->Draw(worldTransform, *ViewProjection::GetInstance()); }
	void SetTranslation(Vector3 translation) { worldTransform.translation_ = translation; }
	void OnCollision(IncludeCollider* collider) { isGoal = true; }
	static void SetIsGoal(bool isGoal_) { isGoal = isGoal_; }
	static bool IsGoal() { return isGoal; }
};

class StagePlane : public PlaneCollider
{
	StagePlane() = default;
	UINT16* nowStage = nullptr;
public:
	static StagePlane* GetInstance();
	void Initialize();
	void Rotation(Quaternion rotQ) { normal = RotateVector(normal, rotQ); }
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
	//int num_ = 0;
public:
	bool IsDestroy() { return isDestroy; }
	void Initialize();
	void Update();
	void Draw() {}
	void SetNum(int num) { num_ = num; }
};