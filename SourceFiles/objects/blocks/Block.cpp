#include <imgui.h>
#include "Block.h"
#include "ImGuiManager.h"
#include "CollisionManager.h"
#include "Mouse.h"

#pragma region BaseBlock
UINT16* BaseBlock::nowStage = nullptr;

void BaseBlockCollider::Initialize()
{
	BaseBlock::Initialize();
	SetCollisionAttribute(CollisionAttribute::Block);
	SetCollisionMask(CollisionMask::Block);
	worldTransform.Initialize();
}
#pragma endregion

#pragma region NormalBlock
void NonCollisionNormalBlock::Initialize()
{
	BaseBlock::Initialize();
	worldTransform.Initialize();
	worldTransform.Update();
	SetTexture("normalBlock.png");
}

void NormalBlock::Initialize()
{
	BaseBlockCollider::Initialize();
	worldTransform.Update();
	SetTexture("normalBlock.png");
}
#pragma endregion

#pragma region MoveBlock
void MoveBlock::Initialize()
{
	BaseBlockCollider::Initialize();
	SetVertices();
	SetTexture("moveBlock.png");
	normal = RotateVector({ 0,0,-1 }, CubeQuaternion::Get(*nowStage));
}

void MoveBlock::DragBox()
{
	if (!isDrag) { return; }

	worldTransform.translation_ = *StagePlane::GetInstance()->GetInter();

	// ƒNƒŠƒbƒN‚ª—£‚³‚ê‚½‚Æ‚«
	if (!input->IsPressMouse(0)) { isDrag = false; }
}

void MoveBlock::Update()
{
	DragBox();
	worldTransform.Update();
	normal = RotateVector({ 0,0,-1 }, CubeQuaternion::Get(*nowStage));
	SetVertices();
}
#pragma endregion

#pragma region CopyBlock
void CopyBlock::Initialize()
{
	BaseBlockCollider::Initialize();
	SetTexture("copyBlock.png");
	normal = RotateVector({ 0,0,-1 },CubeQuaternion::Get(*nowStage));
	SetVertices();
}

std::unique_ptr<BaseBlock> CopyBlock::NewBlockCreate()
{
	if (isCopy == false) { return nullptr; }
	if (!isCopyMode) { return nullptr; }
	if (!input->IsTriggerMouse(0)) { return nullptr; }
	isCopyMode = false;
	std::unique_ptr<BaseBlock> newBlock = std::make_unique<CopyedBlock>();

	CollisionManager::CheckCollisionRayPlane(Mouse::GetInstance(), StagePlane::GetInstance());
	newBlock->SetTranslation(*StagePlane::GetInstance()->GetInter());
	newBlock->Initialize();

	isCopy = false;
	return newBlock;
}

void CopyBlock::Update()
{
	if (!isCopy) { SetTexture("copyBlock_2.png"); }
	worldTransform.Update();
}

void CopyedBlock::Initialize()
{
	BaseBlockCollider::Initialize();
	SetTexture("copyedBlock.png");
}
#pragma endregion

#pragma region DestroyBlock
void DestroyBlock::Initialize()
{
	BaseBlockCollider::Initialize();
	SetTexture("destroyBlock.png");
	normal = RotateVector({ 0,0,-1 }, CubeQuaternion::Get(*nowStage));
	SetVertices();
}

void DestroyBlock::Update()
{
	worldTransform.Update();
	SetVertices();
}

void DestroyBlock::OnCollision(RayCollider* collider)
{
	clickNum += input->IsTriggerMouse(0);

	if (clickNum == 1) { SetTexture("destroyBlock_2.png"); }
	if (clickNum == 2) { SetTexture("destroyBlock_1.png"); }
	if (clickNum >= DESTROY_NUM) { isDestroy = true; }
}
#pragma endregion

#pragma region LadderBlock
void LadderBlock::Initialize()
{
	model = Model::CreateFromOBJ("ladder");
	SetUseAxis(Axis::Y, false);
	worldTransform.Initialize();
	worldTransform.Update();
	SetCollisionAttribute(CollisionAttribute::LadderBlock);
	SetCollisionMask(CollisionMask::Block);
}
#pragma endregion

#pragma region ElseBlock
StagePlane* StagePlane::GetInstance()
{
	static StagePlane instance;
	return &instance;
}

void StagePlane::Initialize()
{
	distance = 39.0f;
	normal = RotateVector({ 0,0,-1 }, CubeQuaternion::Get(*nowStage));
	SetCollisionAttribute(CollisionAttribute::StagePlane);
	SetCollisionMask(CollisionMask::StagePlane);
}

void BgBlock::Initialize()
{
	BaseBlock::Initialize();
	worldTransform.Initialize();
	worldTransform.scale_ *= 38.0f;
	worldTransform.Update();
	SetTexture("bgBlock.png");
}
#pragma endregion

void Button::Initialize()
{
	model = Model::CreateFromOBJ("botton");
	worldTransform.Initialize();
	worldTransform.Update();
	SetCollisionAttribute(CollisionAttribute::Button);
	SetCollisionMask(CollisionMask::Block);
}

uint16_t Button::useCount;

void Button::OnCollision(BoxCollider* collider)
{
	isDestroy = true;
	useCount++;
}

bool GoalBlock::isGoal = false;

void GoalBlock::Initialize()
{
	model = Model::CreateFromOBJ("gate");
	worldTransform.rotation_.y = PI / 2.0f * (*nowStage - 1);
	worldTransform.Initialize();
	worldTransform.Update();
	SetCollisionAttribute(CollisionAttribute::GoalBlock);
	SetCollisionMask(CollisionMask::Block);
}

#pragma region StopBlock
void StopBlock::Initialize()
{
	BaseBlockCollider::Initialize();
	SetTexture("copyedBlock.png");
}

void StopBlock::Update()
{
	if (num_ == Button::GetUseCount()) { isDestroy = true; }
	worldTransform.Update();
}
#pragma endregion
