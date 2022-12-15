#include "GamePlayScene.h"
#include "GameScene.h"
#include "ImGuiManager.h"
#include "WinApp.h"
#include "DirectXCommon.h"

void DragBox(WorldTransform& w, bool& isDrag)
{
	// �C���X�^���X�̎擾
	ViewProjection* viewProjection = ViewProjection::GetInstance();
	Input* input = Input::GetInstance();
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();

	// w�̍��W���擾
	Vector3 blockPos = w.translation_;
	// �r���[�|�[�g�s��̐���
	Matrix4 matViewPort = Matrix4Identity();
	matViewPort.m[0][0] = (float)WinApp::kWindowWidth / 2;
	matViewPort.m[1][1] = -(float)WinApp::kWindowHeight / 2;
	matViewPort.m[3][0] = (float)WinApp::kWindowWidth / 2;
	matViewPort.m[3][1] = (float)WinApp::kWindowHeight / 2;
	// �r���[�A�v���W�F�N�V�����A�r���[�|�[�g�s��̊|���Z
	Matrix4 mat = viewProjection->matView * viewProjection->matProjection * matViewPort;
	// blocks[4]��2D���W�A���a���v�Z
	Vector3 block2DRad = Vector3TransformCoord(blockPos - w.scale_, mat);
	Vector3 block2DPos = Vector3TransformCoord(blockPos, mat);
	// �}�E�X���W���擾
	Vector2 mousePos = input->GetMousePosition();
	// �}�E�X���W��w��2D���W�������ɂ�������
	if (isDrag || (fabs(block2DPos.x - (float)mousePos.x) <= fabs(block2DPos.x - block2DRad.x) &&
		fabs(block2DPos.y - (float)mousePos.y) <= fabs(block2DPos.y - block2DRad.y)))
	{
		// �}�E�X�̍��{�^����������Ă���ꍇ
		if (input->IsPressMouse(0))
		{
			isDrag = true;
			// �X�N���[�����W
			Vector3 posNear = { mousePos.x,mousePos.y,0 };
			Vector3 posFar = { mousePos.x,mousePos.y,1 };
			// �X�N���[�����W�����[���h���W�ɕϊ�
			posNear = Vector3TransformCoord(posNear, Matrix4Inverse(mat));
			posFar = Vector3TransformCoord(posFar, Matrix4Inverse(mat));
			// �}�E�X���C�̕���
			Vector3 mouseDirection = posFar - posNear;
			mouseDirection = Vector3Normalize(mouseDirection);
			// �J��������blocks[4]�̋���
			float distanceObject = Vector3Length(viewProjection->eye - blockPos);
			w.translation_ = posNear + mouseDirection * distanceObject;
			w.translation_.z = 0;
		}
		else { isDrag = false; }
	}

	imguiManager->SliderVector("blockPos", blockPos);
	imguiManager->PrintVector("mousePos", mousePos);
}

void GamePlayScene::Initialize()
{
	gameScene = GameScene::GetInstance();

	viewProjection->eye.y = 10.0f;
	viewProjection->target.y = 10.0f;

	model_ = Model::Create();

	blocks.resize(15);
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

	for (WorldTransform& w : blocks) { w.Initialize(); }

	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	player_.Initialize();
}

void GamePlayScene::Update()
{
	player_.Update();

	// �����蔻��
	collisionManager.CheckAllCollisions(&player_);

	for (size_t i = 0; i < blocks.size(); i++)
	{
		DragBox(blocks[i], isDrag);
	}

	for (WorldTransform& w : blocks) { w.Update(); }

	debugCamera_->Update();
	//*viewProjection_ = debugCamera_->GetViewProjection();
}

void GamePlayScene::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* commandList = DirectXCommon::GetInstance()->GetCommandList();

	// 3D�I�u�W�F�N�g�`��O����
	Model::PreDraw(commandList);

	for (WorldTransform& w : blocks) { model_->Draw(w, *viewProjection); }

	// 3D�I�u�W�F�N�g�`��㏈��
	Model::PostDraw();
}

void GamePlayScene::Finalize()
{
	delete model_;
}