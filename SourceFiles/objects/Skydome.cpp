#include "Skydome.h"


void Skydome::Initialize()
{
	skydome = Model::CreateFromOBJ("skydome");
	worldTransform_.Initialize();
	worldTransform_.scale_ = { 200,200,200 };
}

void Skydome::Update()
{
	worldTransform_.Update();
}

void Skydome::Draw()
{
	skydome->Draw(worldTransform_, *ViewProjection::GetInstance());
}