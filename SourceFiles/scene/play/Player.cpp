#include "Player.h"
#include "ImGuiManager.h"
#include <imgui.h>

Player* Player::GetInstance()
{
	static Player instance;
	return &instance;
}

void Player::Initialize()
{
	model_ = Model::Create();
	worldTransform.Initialize();
	worldTransform.translation_ = { 20.0f ,2.0f+epsilon,0 };
}

void Player::Move()
{
	worldTransform.translation_.x += spdX;
}

void Player::Update()
{
	Move();

	worldTransform.Update();

	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	imguiManager->InputVector("playerPos", worldTransform.translation_);
}

void Player::Draw()
{
	model_->Draw(worldTransform, *ViewProjection::GetInstance());
}

void Player::OnCollision(Collider* collider)
{
	spdX = -spdX;
}