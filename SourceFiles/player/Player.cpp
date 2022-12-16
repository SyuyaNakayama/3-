#include "Player.h"

Player* Player::GetInstance()
{
	static Player instance;
	return &instance;
}

void Player::Initialize()
{
	model_ = Model::Create();
	worldTransform.Initialize();
	worldTransform.translation_.x = -10.0f;
}

void Player::Move()
{

}

void Player::Update()
{
	Move();
	worldTransform.Update();
}

void Player::Draw()
{
	model_->Draw(worldTransform, *ViewProjection::GetInstance());
}
