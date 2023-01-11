#include "BlockManager.h"

BlockManager* BlockManager::GetInstance()
{
	static BlockManager instance;
	return &instance;
}

void BlockManager::CreateBlock(Vector3 pos, Vector3 scale, BlockType type)
{
	std::unique_ptr<BaseBlock> block;

	switch (type)
	{
	case BlockManager::BlockType::Copy:
		block = std::make_unique<CopyBlock>();
		break;
	case BlockManager::BlockType::Move:
		block = std::make_unique<MoveBlock>();
		break;
	case BlockManager::BlockType::Destroy:
		block = std::make_unique<DestroyBlock>();
		break;
	}
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
		CreateBlock({ 0,0,0 }, { 25,1,1 }, BlockType::Destroy);
		CreateBlock({ 15.0f,2.0f,0 }, { 1,1,1 }, BlockType::Move);
		CreateBlock({ 13.0f,3.0f,0 }, { 1,2,1 }, BlockType::Move);
		CreateBlock({ 11.0f,4.0f,0 }, { 1,3,1 }, BlockType::Move);
		//CreateBlock({ 5.0f,10.0f,0 }, { 1,3,1 });
		CreateBlock({ 7.0f,4.0f,0 }, { 1,3,1 }, BlockType::Move);
		CreateBlock({ 5.0f,4.0f,0 }, { 1,3,1 }, BlockType::Move);
		CreateBlock({ 3.0f,4.0f,0 }, { 1,3,1 }, BlockType::Move);
		CreateBlock({ 1.0f,3.0f,0 }, { 1,2,1 }, BlockType::Move);
		CreateBlock({ -1.0f,2.0f,0 }, { 1,1,1 }, BlockType::Move);
		CreateBlock({ 5.0f,17.0f,0 }, { 1,4,1 }, BlockType::Move);
		CreateBlock({ 0,22,0 }, { 25,1,1 }, BlockType::Move);
		CreateBlock({ 25.0f,2.0f,0 }, { 1,1,1 }, BlockType::Move);
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
	blocks.remove(nullptr);
	for (std::unique_ptr<BaseBlock>& block : blocks) { block->Update(); }
}

void BlockManager::Draw()
{
	for (std::unique_ptr<BaseBlock>& block : blocks) { block->Draw(); }
}