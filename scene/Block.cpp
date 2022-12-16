#include "Block.h"
#include "ViewProjection.h"

void Block::Initialize()
{
	worldTransform.Initialize();
	model = Model::Create();
}

void Block::Update()
{
	worldTransform.Update();
}

void Block::Draw()
{
	model->Draw(worldTransform, *ViewProjection::GetInstance());
}
