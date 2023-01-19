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

void BlockManager::Initialize(UINT16 stage)
{
	switch (stage)
	{
	case 0: LoadMap("stage1.txt"); break;
	case 1: LoadMap("stage2.txt"); break;
	case 2: LoadMap("stage3.txt"); break;
	case 3: LoadMap("stage4.txt"); break;
	}
	StagePlane::GetInstance()->Initialize();
	unique_ptr<BaseBlock> bgBlock = make_unique<BgBlock>();
	bgBlock->SetTranslation({ 40,-40,40 });
	bgBlock->Initialize();
	blocks.push_back(move(bgBlock));
}

void BlockManager::Update()
{
	blocks.remove_if([](const unique_ptr<BaseBlock>& block) { return block->IsDestroy(); });
	for (const unique_ptr<BaseBlock>& block : blocks) { block->Update(); }
}

void BlockManager::Draw()
{
	for (const unique_ptr<BaseBlock>& block : blocks) { block->Draw(); }
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
	case 7: return BlockType::NonCollisionNormal;
	default: return BlockType::None;
	}
}

void BlockManager::CreateBlocks()
{
	for (UINT16 y = 0; y < STAGE_SIZE; y++) {
		for (UINT16 x = 0; x < STAGE_SIZE; x++)
		{
			CreateBlock({ 2.0f * x, -2.0f * y, 0 }, map[y][x]);
		}
	}
}

void BlockManager::CreateBlock(Vector3 pos, BlockType type)
{
	unique_ptr<BaseBlock> block;
	switch (type)
	{
	case BlockType::None: return;
	case BlockType::Normal:				block = make_unique<NormalBlock>();				break;
	case BlockType::Move:				block = make_unique<MoveBlock>();				break;
	case BlockType::Copy:				block = make_unique<CopyBlock>();				break;
	case BlockType::Destroy:			block = make_unique<DestroyBlock>();			break;
	case BlockType::Ladder:				block = make_unique<MoveBlock>();				break;
	case BlockType::Goal:				block = make_unique<MoveBlock>();				break;
	case BlockType::NonCollisionNormal:	block = make_unique<NonCollisionNormalBlock>();	break;
	}
	assert(block);
	block->SetTranslation(pos);
	block->Initialize();
	blocks.push_back(move(block));
}