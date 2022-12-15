#include "Player.h"

void Player::Initialize()
{
	model_ = Model::Create();
	input_ = Input::GetInstance();
	worldTransform_.Initialize();
	worldTransform_.translation_.x = -10.0f;
}

void Player::Move()
{

}

void Player::Update()
{
	Move();
	worldTransform_.Update();
}

void Player::Draw()
{
	model_->Draw(worldTransform_, *ViewProjection::GetInstance());
}
