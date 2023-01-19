#include "Mouse.h"
#include "WinApp.h"
#include "ImGuiManager.h"
#include "Collision.h"
#include <imgui.h>
using namespace MathUtility;

Mouse* Mouse::GetInstance()
{
	static Mouse instance;
	return &instance;
}

void Mouse::Update()
{
	ViewProjection* viewProjection = ViewProjection::GetInstance();
	
	Vector2 mousePos = Input::GetInstance()->GetMousePosition();

	// ビュー、プロジェクション、ビューポート行列の掛け算
	Matrix4 mat = viewProjection->matView * viewProjection->matProjection * matViewPort;
	// スクリーン座標をワールド座標に変換
	posNear = Vector3TransformCoord({ mousePos.x,mousePos.y,0 }, Matrix4Inverse(mat));
	Vector3 posFar = Vector3TransformCoord({ mousePos.x,mousePos.y,1 }, Matrix4Inverse(mat));

	// マウスレイの方向
	direction = posFar - posNear;
	direction.normalize();
}

void Mouse::Initialize()
{
	matViewPort.m[0][0] = (float)WinApp::kWindowWidth / 2;
	matViewPort.m[1][1] = -(float)WinApp::kWindowHeight / 2;
	matViewPort.m[3][0] = (float)WinApp::kWindowWidth / 2;
	matViewPort.m[3][1] = (float)WinApp::kWindowHeight / 2;

	SetCollisionAttribute(CollisionAttribute::MouseRay);
	SetCollisionMask(CollisionMask::MouseRay);
}