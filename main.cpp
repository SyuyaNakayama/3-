#include "GameScene.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) 
{	
	// ゲームウィンドウの作成
	WinApp* win = WinApp::GetInstance();
	win->CreateGameWindow("2347_箱庭奇譚");

	// ゲームシーンの初期化
	GameScene* gameScene = GameScene::GetInstance();
	gameScene->Initialize();

	// メインループ
	while (!win->ProcessMessage())
	{
		gameScene->Update();
		gameScene->Draw();
	}

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();

	return 0;
}