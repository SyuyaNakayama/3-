#include "Block.h"
#include "ViewProjection.h"
#include "Input.h"
#include "ImGuiManager.h"
#include "WinApp.h"

void DragBox(WorldTransform& w, bool& isDrag)
{
	// インスタンスの取得
	ViewProjection* viewProjection = ViewProjection::GetInstance();
	Input* input = Input::GetInstance();
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();

	// wの座標を取得
	Vector3 blockPos = w.translation_;
	// ビューポート行列の生成
	Matrix4 matViewPort = Matrix4Identity();
	matViewPort.m[0][0] = (float)WinApp::kWindowWidth / 2;
	matViewPort.m[1][1] = -(float)WinApp::kWindowHeight / 2;
	matViewPort.m[3][0] = (float)WinApp::kWindowWidth / 2;
	matViewPort.m[3][1] = (float)WinApp::kWindowHeight / 2;
	// ビュー、プロジェクション、ビューポート行列の掛け算
	Matrix4 mat = viewProjection->matView * viewProjection->matProjection * matViewPort;
	// blocks[4]の2D座標、半径を計算
	Vector3 block2DRad = Vector3TransformCoord(blockPos - w.scale_, mat);
	Vector3 block2DPos = Vector3TransformCoord(blockPos, mat);
	// マウス座標を取得
	Vector2 mousePos = input->GetMousePosition();
	// マウス座標がwの2D座標より内側にあったら
	if (isDrag || (fabs(block2DPos.x - (float)mousePos.x) <= fabs(block2DPos.x - block2DRad.x) &&
		fabs(block2DPos.y - (float)mousePos.y) <= fabs(block2DPos.y - block2DRad.y)))
	{
		// マウスの左ボタンが押されている場合
		if (input->IsPressMouse(0))
		{
			isDrag = true;
			// スクリーン座標
			Vector3 posNear = { mousePos.x,mousePos.y,0 };
			Vector3 posFar = { mousePos.x,mousePos.y,1 };
			// スクリーン座標をワールド座標に変換
			posNear = Vector3TransformCoord(posNear, Matrix4Inverse(mat));
			posFar = Vector3TransformCoord(posFar, Matrix4Inverse(mat));
			// マウスレイの方向
			Vector3 mouseDirection = posFar - posNear;
			mouseDirection = Vector3Normalize(mouseDirection);
			// カメラからblocks[4]の距離
			float distanceObject = Vector3Length(viewProjection->eye - blockPos);
			w.translation_ = posNear + mouseDirection * distanceObject;
			w.translation_.z = 0;
		}
		else { isDrag = false; }
	}
}

void Block::Initialize()
{
	worldTransform.Initialize();
	model = Model::Create();
}

void Block::Update()
{
	DragBox(worldTransform, isDrag);
	worldTransform.Update();
}

void Block::Draw()
{
	model->Draw(worldTransform, *ViewProjection::GetInstance());
}
