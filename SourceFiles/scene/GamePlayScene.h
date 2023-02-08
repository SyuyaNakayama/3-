#pragma once
#include "BaseScene.h"
#include "Player.h"
#include "DebugCamera.h"
#include "Mouse.h"

class GamePlayScene : public BaseScene
{
private:
	Player player_;
	std::unique_ptr<DebugCamera> debugCamera_;
	Mouse* mouse = Mouse::GetInstance();
	Vector3 eyePos[2] = { { 20,-20,-600 },{ 0,0,-1200 } };
	Vector3 preEyePos{}; // カメラ補間前のeyeの場所
	Vector3 targetPos[2] = { { 20,-20,0 },{ 0,0,0 } };
	bool isCameraLerp = false;
	float t = 0; // 補間に使う変数
	float dt = 0.01f; // tの増加量
	UINT16* stage = nullptr;

public:
	void Initialize() override;
	void Finalize() {};
	void Update() override;
	void Draw() override;
	// カメラ補間(カメラ移動中はtrueを返す)
	bool CameraLerp(bool isFlip = false); // 視野を広げる(狭める)補間
	bool ChangeNextStage(); // 次のステージに遷移する補間
};