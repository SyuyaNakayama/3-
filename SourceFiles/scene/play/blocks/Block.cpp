#include "Block.h"
#include "ViewProjection.h"
#include "Input.h"
#include "ImGuiManager.h"
#include "WinApp.h"

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

	// blockの座標を取得
	Vector3 blockPos = worldTransform.GetWorldPosition();
	// ビューポート行列の生成
	Matrix4 matViewPort = Matrix4Identity();
	matViewPort.m[0][0] = (float)WinApp::kWindowWidth / 2;
	matViewPort.m[1][1] = -(float)WinApp::kWindowHeight / 2;
	matViewPort.m[3][0] = (float)WinApp::kWindowWidth / 2;
	matViewPort.m[3][1] = (float)WinApp::kWindowHeight / 2;
	// ビュー、プロジェクション、ビューポート行列の掛け算
	Matrix4 mat = viewProjection->matView * viewProjection->matProjection * matViewPort;
	// blockの2D座標、半径を計算
	Vector3 block2DRad = Vector3TransformCoord(blockPos - worldTransform.scale_, mat);
	Vector3 block2DPos = Vector3TransformCoord(blockPos, mat);
	// マウス座標を取得
	Vector2 mousePos = input->GetMousePosition();
	// マウス座標がblockの2D座標より内側にあったら
	bool flag = isDrag || (fabs(block2DPos.x - (float)mousePos.x) <= fabs(block2DPos.x - block2DRad.x) &&
		fabs(block2DPos.y - (float)mousePos.y) <= fabs(block2DPos.y - block2DRad.y));
	if (!flag) { return; }

	// マウスの左ボタンが押された場合
	if (input->IsTriggerMouse(0)) { isDrag = true; }
	if (!isDrag) { return; }

	// スクリーン座標
	Vector3 posNear = { mousePos.x,mousePos.y,0 };
	Vector3 posFar = { mousePos.x,mousePos.y,1 };
	// スクリーン座標をワールド座標に変換
	posNear = Vector3TransformCoord(posNear, Matrix4Inverse(mat));
	posFar = Vector3TransformCoord(posFar, Matrix4Inverse(mat));
	// マウスレイの方向
	Vector3 mouseDirection = posFar - posNear;
	mouseDirection = Vector3Normalize(mouseDirection);
	// カメラからblockの距離
	float distanceObject = Vector3Length(viewProjection->eye - blockPos);
	worldTransform.translation_ = posNear + mouseDirection * distanceObject;
	worldTransform.translation_.z = 0;
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
		delete this; 
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
	Destroy();
	worldTransform.Update();
}