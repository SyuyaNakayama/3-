#include "Enemy.h"

void Enemy::Move()
{
}

void Enemy::Initialize()
{
	model_ = Model::Create();
	worldTransform_.Initialize();
}

void Enemy::Update()
{
}

void Enemy::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}
