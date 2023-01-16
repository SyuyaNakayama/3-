#include "Block.h"
#include "ViewProjection.h"
#include "ImGuiManager.h"
#include "WinApp.h"
#include "Mouse.h"
#include "BlockManager.h"
#include <imgui.h>

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


void NonCollisionNormalBlock::Initialize()
{
	BaseBlock::Initialize();
	worldTransform.Initialize();
	//SetTexture("normalBlock.png");
}

void NormalBlock::Initialize()
{
	BaseBlockCollider::Initialize();
	//SetTexture("normalBlock.png");
}


void MoveBlock::DragBox()
{
	if (!isDrag) { return; }

	worldTransform.translation_ = *StagePlane::GetInstance()->GetInter();

	// ƒNƒŠƒbƒN‚ª—£‚³‚ê‚½‚Æ‚«
	if (!input->IsPressMouse(0)) { isDrag = false; }
}

void MoveBlock::Initialize()
{
	BaseBlockCollider::Initialize();
	SetTexture("moveBlock.png");
	normal = { 0,0,-1 };
}

void MoveBlock::Update()
{
	DragBox();
	BoxCollider::worldTransform.Update();
	SetVertices();
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
}

void MoveBlock::OnCollision(RayCollider* Collider)
{
	ImGui::Text("Hit!!");
	if (Input::GetInstance()->IsTriggerMouse(0)) { isDrag = true; }
}


void CopyBlock::Initialize()
{
	BaseBlockCollider::Initialize();
	SetTexture("copyBlock.png");
}

void CopyBlock::Update()
{
	worldTransform.Update();
}


void DestroyBlock::Destroy()
{
	clickNum += input->IsTriggerMouse(0);
	if (clickNum >= DESTROY_NUM)
	{
		std::list<BaseBlock*>& blocks = BlockManager::GetInstance()->GetBlocks();
		auto itr = blocks.begin();
		for (; itr != blocks.end(); itr++)
		{
			if (*itr == this)
			{
				//blocks.erase(itr);
				blocks.emplace_front(this);
				clickNum = 0;
				return;
			}
		}
	}
}

void DestroyBlock::Initialize()
{
	BaseBlockCollider::Initialize();
	SetTexture("destroyBlock.png");
}

void DestroyBlock::Update()
{
	worldTransform.Update();
	Destroy();
}

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

void StagePlane::OnCollision(RayCollider* Collider)
{
	ImGui::Text("Hit!!");

}