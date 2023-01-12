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

void BlockManager::CreateBlock(Vector3 pos, Vector3 scale, BlockType type)
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
	block->Initialize();
	block->SetTranslation(pos);
	block->SetScale(scale);
	blocks.push_back(block);
}

void BlockManager::CreateBlocks()
{
	for (UINT16 y = 0; y < STAGE_SIZE; y++)
	{
		for (UINT16 x = 0; x < STAGE_SIZE; x++)
		{
			CreateBlock({ 2.0f * x, 2.0f * y, 0 }, { 1,1,1 }, map[y][x]);
		}
	}
}

void BlockManager::Initialize(UINT16 stage)
{
	switch (stage)
	{
	case 0:
		LoadMap("stage1.txt");

		//CreateBlock({ 0,0,0 }, { 25,1,1 }, BlockType::Destroy);
		//CreateBlock({ 15.0f,2.0f,0 }, { 1,1,1 }, BlockType::Move);
		//CreateBlock({ 13.0f,3.0f,0 }, { 1,2,1 }, BlockType::Move);
		//CreateBlock({ 11.0f,4.0f,0 }, { 1,3,1 }, BlockType::Move);
		//CreateBlock({ 5.0f,10.0f,0 }, { 1,3,1 });
		//CreateBlock({ 7.0f,4.0f,0 }, { 1,3,1 }, BlockType::Move);
		//CreateBlock({ 5.0f,4.0f,0 }, { 1,3,1 }, BlockType::Move);
		//CreateBlock({ 3.0f,4.0f,0 }, { 1,3,1 }, BlockType::Move);
		//CreateBlock({ 1.0f,3.0f,0 }, { 1,2,1 }, BlockType::Move);
		//CreateBlock({ -1.0f,2.0f,0 }, { 1,1,1 }, BlockType::Move);
		//CreateBlock({ 5.0f,17.0f,0 }, { 1,4,1 }, BlockType::Move);
		//CreateBlock({ 0,22,0 }, { 25,1,1 }, BlockType::Move);
		//CreateBlock({ 25.0f,2.0f,0 }, { 1,1,1 }, BlockType::Move);
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
	for (size_t i = 0; i < STAGE_SIZE; i++)
	{
		for (size_t j = 0; j < STAGE_SIZE; j++)
		{
			ImGui::Text("%d", map[i][j]);
		}
	}

	for (BaseBlock* block : blocks)
	{
		block->Update();
	}
	ImGui::Text("%d", blocks.size());
}

void BlockManager::Draw()
{
	for (BaseBlock* block : blocks)
	{
		block->Draw();
	}
}