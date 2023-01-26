#include "Block.h"
#include "ViewProjection.h"
#include "ImGuiManager.h"
#include <imgui.h>

#pragma region BaseBlock
void BaseBlock::Initialize() { model = Model::Create(); }
void BaseBlock::SetTexture(const std::string& fileName) { textureHandle = TextureManager::Load("blockTextures/" + fileName); }

void BaseBlockCollider::Initialize()
{
	BaseBlock::Initialize();
	SetCollisionAttribute(CollisionAttribute::Block);
	SetCollisionMask(CollisionMask::Block);
	worldTransform.Initialize();
}

void BaseBlockCollider::Draw() { model->Draw(worldTransform, *ViewProjection::GetInstance(), textureHandle); }
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
	normal = { 0,0,-1 };
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
	SetVertices();
}

void MoveBlock::OnCollision(RayCollider* Collider)
{
	if (Input::GetInstance()->IsTriggerMouse(0)) { isDrag = true; }
}
#pragma endregion

#pragma region CopyBlock
void CopyBlock::Initialize()
{
	BaseBlockCollider::Initialize();
	SetTexture("copyBlock.png");
	SetVertices();
	normal = { 0,0,-1 };
}

std::unique_ptr<BaseBlock> CopyBlock::NewBlockCreate()
{
	if (!isCopyMode) { return nullptr; }
	if (!input->IsTriggerMouse(0)) { return nullptr; }
	isCopyMode = false;

	std::unique_ptr<BaseBlock> newBlock = std::make_unique<CopyBlock>();
	newBlock->SetTranslation(*StagePlane::GetInstance()->GetInter());
	newBlock->Initialize();
	return newBlock;
}

void CopyBlock::Update()
{
	worldTransform.Update();
}

void CopyBlock::OnCollision(RayCollider* Collider) { if (input->IsTriggerMouse(0)) { isCopyMode = true; } }
#pragma endregion

#pragma region DestroyBlock
void DestroyBlock::Initialize()
{
	BaseBlockCollider::Initialize();
	SetTexture("destroyBlock.png");
	SetVertices();
	normal = { 0,0,-1 };
}

void DestroyBlock::Update()
{
	worldTransform.Update();
	SetVertices();
}

void DestroyBlock::OnCollision(RayCollider* collider)
{
	clickNum += input->IsTriggerMouse(0);
	if (clickNum >= DESTROY_NUM) { isDestroy = true; }
}
#pragma endregion

#pragma region LadderBlock
void LadderBlock::Initialize()
{
	model = Model::CreateFromOBJ("ladder");
	SetUseAxis(Axis::Y, false);
	worldTransform.Initialize();
	worldTransform.translation_.z = -1.0f;
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
	distance = 0;
	normal = { 0,0,-1 };
	SetCollisionAttribute(CollisionAttribute::StagePlane);
	SetCollisionMask(CollisionMask::StagePlane);
}

void BgBlock::Initialize()
{
	BaseBlock::Initialize();
	worldTransform.Initialize();
	worldTransform.scale_ *= 39.0f;
	worldTransform.translation_ = { 40,-40,40 };
	worldTransform.Update();
	SetTexture("bgBlock.png");
}
#pragma endregion

void Button::Initialize()
{
	model = Model::CreateFromOBJ("botton");
	worldTransform.Initialize();
	worldTransform.rotation_.x = -PI / 2.0f;
	worldTransform.translation_.z = -1.0f;
	worldTransform.Update();
	SetCollisionAttribute(CollisionAttribute::Button);
	SetCollisionMask(CollisionMask::Block);
}

void Button::OnCollision(BoxCollider* collider)
{
	isDraw = false;
}
