#include "GameScene.h"
#include "Quaternion.h"

UINT16 GameScene::stage = 4;

void GameScene::Initialize()
{
	// 入力の初期化
	input->Initialize();

	// オーディオの初期化
	audio->Initialize();

	dxCommon->Initialize(WinApp::GetInstance());

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 3Dモデル静的初期化
	Model::StaticInitialize();

	viewProjection->Initialize();
	viewProjection->fovAngleY = 4.0f * MathUtility::PI / 180.0f;
	viewProjection->farZ = 1500.0f;

	CubeQuaternion::Create();

	// シーンの生成
	SetNextScene(Scene::Play, false);
	imguiManager->Initialize();
	fadeManager_.Initialize();
}

void GameScene::Update()
{
	// 入力関連の毎フレーム処理
	input->Update();

	imguiManager->Begin();
	// ゲームシーンの毎フレーム処理
	fadeManager_.Update();

	if (fadeManager_.IsChange() || !scene_)
	{
		if (nextScene_ != Scene::Null)
		{
			if (scene_)
			{
				scene_->Finalize();
				delete scene_;
			}

			scene_ = sceneFactory_->CreateScene(nextScene_);
			nextScene_ = Scene::Null;
			scene_->SetGameScene(this);
			scene_->Initialize();
		}
	}

	if (!fadeManager_.IsFade())
	{
		if (input->PushKey(DIK_1)) 
		{
			for (size_t i = 0; i < 10; i++)
			{
			scene_->Update();
			}
		}
		scene_->Update();
	}

	imguiManager->End();

	viewProjection->UpdateMatrix();
}

void GameScene::Draw()
{
	// 描画開始
	dxCommon->PreDraw();
	// ゲームシーンの描画
	scene_->Draw();
	fadeManager_.Draw();
	// デバッグの描画(ImGui)
	imguiManager->Draw();
	// 描画終了
	dxCommon->PostDraw();
}

GameScene* GameScene::GetInstance()
{
	static GameScene instance;
	return &instance;
}

void GameScene::SetNextScene(Scene nextScene, bool isUseFade)
{
	nextScene_ = nextScene;
	if (isUseFade) { fadeManager_.FadeScene(); }
}

GameScene::~GameScene()
{
	scene_->Finalize();
	delete scene_;
	audio->Finalize();
	imguiManager->Finalize();
}