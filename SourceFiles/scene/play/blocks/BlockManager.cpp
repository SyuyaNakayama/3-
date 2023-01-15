#include "BlockManager.h"
#include <imgui.h>
#include <fstream>
#include <sstream>
using namespace std;

BlockManager* BlockManager::GetInstance()
{
	static BlockManager instance;
	return &instance;
}

BlockManager::BlockType BlockManager::IntToBlockType(int num)
{
	switch (num)
	{
	case 0: return BlockType::None;
	case 1: return BlockType::Normal;
	case 2: return BlockType::Move;
	case 3: return BlockType::Copy;
	case 4: return BlockType::Destroy;
	case 5: return BlockType::Ladder;
	case 6: return BlockType::Goal;
	default: return BlockType::None;
	}
}

void BlockManager::LoadMap(const std::string& fileName)
{
	fstream file;
	file.open("Resources/stages/" + fileName);
	assert(file.is_open());

	string line, key;
	for (UINT16 y = 0; y < STAGE_SIZE;)
	{
		getline(file, line);
		if (line.empty()) { continue; }
		istringstream lineStream(line);

		for (UINT16 x = 0; x < STAGE_SIZE; x++)
		{
			int temp;
			lineStream >> temp;
			map[y][x] = IntToBlockType(temp);
			getline(lineStream, key, ',');
		}
		y++;
	}
	file.close();

	CreateBlocks();
}

void BlockManager::CreateBlock(Vector3 pos, BlockType type)
{
	BaseBlock* block = nullptr;

	switch (type)
	{
	case BlockManager::BlockType::None:
		return;
	case BlockManager::BlockType::Normal:
		block = new MoveBlock;
		break;
	case BlockManager::BlockType::Move:
		block = new MoveBlock;
		break;
	case BlockManager::BlockType::Copy:
		block = new CopyBlock;
		break;
	case BlockManager::BlockType::Destroy:
		block = new DestroyBlock;
		break;
	case BlockManager::BlockType::Ladder:
		block = new MoveBlock;
		break;
	case BlockManager::BlockType::Goal:
		block = new MoveBlock;
		break;
	}
	assert(block);
	block->SetTranslation(pos);
	block->Initialize();
	blocks.push_back(block);
}

void BlockManager::CreateBlocks()
{
	for (UINT16 y = 0; y < STAGE_SIZE; y++)
	{
		for (UINT16 x = 0; x < STAGE_SIZE; x++)
		{
			CreateBlock({ 2.0f * x, -2.0f * y, 0 }, map[y][x]);
		}
	}
}

void BlockManager::Initialize(UINT16 stage)
{
	switch (stage)
	{
	case 0:
		LoadMap("stage1.txt");
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
	for (BaseBlock* block : blocks)
	{
		block->Update();
	}
}

void BlockManager::Draw()
{
	for (BaseBlock* block : blocks)
	{
		block->Draw();
	}
}