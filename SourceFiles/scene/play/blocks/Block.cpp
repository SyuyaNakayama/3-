#include "Block.h"
#include "ViewProjection.h"
#include "ImGuiManager.h"
#include <imgui.h>
#include "Input.h"
#include "DirectXCommon.h"

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
	CopyblockShadow = Sprite::Create(TextureManager::Load("BlockTextures/copy2.png"),{0,0});
}

std::unique_ptr<BaseBlock> CopyBlock::NewBlockCreate()
{
	if(isCopy == false){return nullptr;}
	if (!isCopyMode) { return nullptr; }
	if (!input->IsTriggerMouse(0)) { return nullptr; }
	isCopyMode = false;
	std::unique_ptr<BaseBlock> newBlock = std::make_unique<CopyedBlock>();

	newBlock->SetTranslation(*StagePlane::GetInstance()->GetInter());
	newBlock->Initialize();

	isCopy = false;
	return newBlock;
}

void CopyBlock::Update()
{
	if (isCopy == false) { SetTexture("copyBlock_2.png"); }
	worldTransform.Update();
	if (isCopyMode) { CopyblockShadow->SetPosition(input->GetMousePosition()); }

}

void CopyBlock::Draw()
{
	if (isCopyMode) {
		Sprite::PreDraw(DirectXCommon::GetInstance()->GetCommandList());
		CopyblockShadow->Draw(); 
		Sprite::PostDraw();
	}
	model->Draw(worldTransform, *ViewProjection::GetInstance(), textureHandle);
}

void CopyBlock::OnCollision(RayCollider* Collider) { if (input->IsTriggerMouse(0)) { isCopyMode = true; }}
#pragma endregion

#pragma region CopyedBlock
void CopyedBlock::Initialize()
{
	BaseBlockCollider::Initialize();
	SetTexture("copyedBlock.png");
}

void CopyedBlock::Update()
{
	worldTransform.Update();
}
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

uint16_t Button::useCount;
void Button::OnCollision(BoxCollider* collider)
{
	isDraw = false;
	isDestroy = true;
	useCount++;
}



#pragma region StopBlock
void StopBlock::Initialize()
{
	BaseBlockCollider::Initialize();
	SetTexture("copyedBlock.png");
}

void StopBlock::Update()
{
	NumDestroy();
	worldTransform.Update();
}

void StopBlock::NumDestroy()
{
	if (num_ == Button::useCount) { isDestroy = true; }
}
#pragma endregion
