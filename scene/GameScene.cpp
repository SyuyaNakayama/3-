﻿#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace std;
using namespace MathUtility;

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

	imguiManager->SliderVector("blockPos", blockPos);
	imguiManager->PrintVector("mousePos", mousePos);
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	model_ = Model::Create();
	imguiManager->Initialize();

	blocks[0].scale_.x = 25.0f;
	blocks[1].translation_ = { 15.0f,2.0f,0 };
	blocks[2].translation_ = { 13.0f,3.0f,0 };
	blocks[2].scale_.y = 2.0f;
	blocks[3].translation_ = { 11.0f,4.0f,0 };
	blocks[3].scale_.y = 3.0f;
	blocks[4].translation_ = { 5.0f,10.0f,0 };
	blocks[4].scale_.y = 3.0f;
	blocks[5].translation_ = { 7.0f,4.0f,0 };
	blocks[5].scale_.y = 3.0f;
	blocks[6].translation_ = { 5.0f,4.0f,0 };
	blocks[6].scale_.y = 3.0f;
	blocks[7].translation_ = { 3.0f,4.0f,0 };
	blocks[7].scale_.y = 3.0f;
	blocks[8].translation_ = { 1.0f,3.0f,0 };
	blocks[8].scale_.y = 2.0f;
	blocks[9].translation_ = { -1.0f,2.0f,0 };
	blocks[10].translation_ = { 5.0f,17.0f,0 };
	blocks[10].scale_.y = 4.0f;
	blocks[11].scale_.x = 25.0f;
	blocks[11].translation_.y = 22.0f;

	viewProjection_->eye.y = 10.0f;
	viewProjection_->target.y = 10.0f;
	for (WorldTransform& w : blocks) { w.Initialize(); }
	player_.Initialize();
}

void GameScene::Update()
{
	imguiManager->Begin();
	player_.Update();

	// 当たり判定
	collisionManager.CheckAllCollisions(&player_);

	DragBox(blocks[4], isDrag);

	for (WorldTransform& w : blocks) { w.Update(); }

	debugCamera_->Update();
	//*viewProjection_ = debugCamera_->GetViewProjection();

	imguiManager->End();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	//player_.Draw();
	for (WorldTransform& w : blocks) { model_->Draw(w, *viewProjection_); }

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグの描画(ImGui)
	imguiManager->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
