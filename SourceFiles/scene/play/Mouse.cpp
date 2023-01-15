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
	ImGuiManager* ImGuiManager = ImGuiManager::GetInstance();
	mousePos = input->GetMousePosition();

	// �X�N���[�����W
	posNear = { mousePos.x,mousePos.y,0 };
	Vector3 posFar = { mousePos.x,mousePos.y,1 };
	Vector3 posO = { mousePos.x,mousePos.y,fabsf(viewProjection->eye.z / viewProjection->farZ) };

	// �r���[�A�v���W�F�N�V�����A�r���[�|�[�g�s��̊|���Z
	mat = viewProjection->matView * viewProjection->matProjection * matViewPort;
	// �X�N���[�����W�����[���h���W�ɕϊ�
	posNear = Vector3TransformCoord(posNear, Matrix4Inverse(mat));
	posFar = Vector3TransformCoord(posFar, Matrix4Inverse(mat));
	posO = Vector3TransformCoord(posO, Matrix4Inverse(mat));

	// �}�E�X���C�̕���
	mouseDirection = posFar - posNear;
	mouseDirection = Vector3Normalize(mouseDirection);

	direction = mouseDirection;

	ImGuiManager->PrintVector("posNear", posNear);
	ImGuiManager->PrintVector("posFar", posFar);
	ImGuiManager->PrintVector("posO", posO);
}

void Mouse::CreateViewPortMatrix()
{
	matViewPort.m[0][0] = (float)WinApp::kWindowWidth / 2;
	matViewPort.m[1][1] = -(float)WinApp::kWindowHeight / 2;
	matViewPort.m[3][0] = (float)WinApp::kWindowWidth / 2;
	matViewPort.m[3][1] = (float)WinApp::kWindowHeight / 2;
}