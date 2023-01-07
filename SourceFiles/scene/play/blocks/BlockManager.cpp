#include "BlockManager.h"

BlockManager* BlockManager::GetInstance()
{
	static BlockManager instance;
	return &instance;
}

void BlockManager::CreateBlock(Vector3 pos, Vector3 scale)
{
	std::unique_ptr<Block> block = std::make_unique<Block>();
	block->Initialize();
	block->SetTranslation(pos);
	block->SetScale(scale);
	blocks.push_back(std::move(block));
}

void BlockManager::Initialize(UINT16 stage)
{
	switch (stage)
	{
	case 0:
		CreateBlock({ 0,0,0 }, { 25,1,1 });
		CreateBlock({ 15.0f,2.0f,0 }, { 1,1,1 });
		CreateBlock({ 13.0f,3.0f,0 }, { 1,2,1 });
		CreateBlock({ 11.0f,4.0f,0 }, { 1,3,1 });
		//CreateBlock({ 5.0f,10.0f,0 }, { 1,3,1 });
		CreateBlock({ 7.0f,4.0f,0 }, { 1,3,1 });
		CreateBlock({ 5.0f,4.0f,0 }, { 1,3,1 });
		CreateBlock({ 3.0f,4.0f,0 }, { 1,3,1 });
		CreateBlock({ 1.0f,3.0f,0 }, { 1,2,1 });
		CreateBlock({ -1.0f,2.0f,0 }, { 1,1,1 });
		CreateBlock({ 5.0f,17.0f,0 }, { 1,4,1 });
		CreateBlock({ 0,22,0 }, { 25,1,1 });
		CreateBlock({ 25.0f,2.0f,0 }, { 1,1,1 });
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	}
}

void BlockManager::Update()
{
	for (std::unique_ptr<Block>& block : blocks) { block->Update(); }
}

void BlockManager::Draw()
{
	for (std::unique_ptr<Block>& block : blocks) { block->Draw(); }
}