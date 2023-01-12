#include "Block.h"
#include "ViewProjection.h"
#include "Input.h"
#include "ImGuiManager.h"
#include "WinApp.h"
#include "Mouse.h"
#include "BlockManager.h"

void BaseBlock::Initialize()
{
	model = Model::Create();
	worldTransform.Initialize();
	SetCollisionAttribute(CollisionAttribute::Block);
	SetCollisionMask(CollisionMask::Block);
}

void BaseBlock::Draw()
{
	model->Draw(worldTransform, *ViewProjection::GetInstance(), textureHandle);
}

void BaseBlock::SetTexture(const std::string& fileName)
{
	textureHandle = TextureManager::Load("blockTextures/" + fileName);
}


void MoveBlock::DragBox()
{
	// インスタンスの取得
	ViewProjection* viewProjection = ViewProjection::GetInstance();
	Input* input = Input::GetInstance();
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	Mouse* mouse = Mouse::GetInstance();

	Matrix4 mat = mouse->GetMatrix();
	Vector2 mousePos = mouse->GetMousePos();
	// blockの座標を取得
	Vector3 blockPos = worldTransform.GetWorldPosition();
	// blockの2D座標、半径を計算
	Vector3 block2DRad = Vector3TransformCoord(blockPos - worldTransform.scale_, mat);
	Vector3 block2DPos = Vector3TransformCoord(blockPos, mat);
	// マウス座標がblockの2D座標より内側にあったら
	bool flag = isDrag || (fabs(block2DPos.x - mousePos.x) <= fabs(block2DPos.x - block2DRad.x) &&
		fabs(block2DPos.y - mousePos.y) <= fabs(block2DPos.y - block2DRad.y));
	if (!flag) { return; }

	// マウスの左ボタンが押された場合
	if (input->IsTriggerMouse(0)) { isDrag = true; }
	if (!isDrag) { return; }

	// カメラからblockの距離
	float distanceObject = Vector3Length(viewProjection->eye - blockPos);
	worldTransform.translation_ = mouse->GetNearPos() + mouse->GetMouseDirection() * distanceObject;
	worldTransform.translation_.z = 0;

	// クリックが離されたとき
	if (!input->IsPressMouse(0)) { isDrag = false; }
}

void MoveBlock::Initialize()
{
	BaseBlock::Initialize();
	SetTexture("moveBlock.png");
}

void MoveBlock::Update()
{
	DragBox();
	worldTransform.Update();
}


void CopyBlock::Initialize()
{
	BaseBlock::Initialize();
	//SetTexture("copyBlock.png");
}

void CopyBlock::Update()
{
	worldTransform.Update();
}


void DestroyBlock::Destroy()
{
	clickNum += Input::GetInstance()->IsTriggerMouse(0);
	if (clickNum >= DESTROY_NUM)
	{
		
	}
}

void DestroyBlock::Initialize()
{
	BaseBlock::Initialize();
	//SetTexture("destroyBlock.png");
	SetTexture("moveBlock.png");
}

void DestroyBlock::Update()
{
	//Destroy();
	worldTransform.Update();
}