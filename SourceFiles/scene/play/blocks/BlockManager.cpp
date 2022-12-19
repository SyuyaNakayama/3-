#include "BlockManager.h"

BlockManager* BlockManager::GetInstance()
{
	static BlockManager instance;
	return &instance;
}

void BlockManager::Initialize(UINT16 stage)
{
	switch (stage)
	{
	case 0:
		blocks.resize(14);
		blocks[0].SetScale({ 25,1,1 });
		blocks[1].SetTranslation({ 15.0f,2.0f,0 });
		blocks[2].SetTranslation({ 13.0f,3.0f,0 });
		blocks[2].SetScale({ 1,2,1 });
		blocks[3].SetTranslation({ 11.0f,4.0f,0 });
		blocks[3].SetScale({ 1,3,1 });
		//blocks[4].SetTranslation({ 5.0f,10.0f,0 });
		//blocks[4].SetScale({ 1,3,1 });
		blocks[5].SetTranslation({ 7.0f,4.0f,0 });
		blocks[5].SetScale({ 1,3,1 });
		blocks[6].SetTranslation({ 5.0f,4.0f,0 });
		blocks[6].SetScale({ 1,3,1 });
		blocks[7].SetTranslation({ 3.0f,4.0f,0 });
		blocks[7].SetScale({ 1,3,1 });
		blocks[8].SetTranslation({ 1.0f,3.0f,0 });
		blocks[8].SetScale({ 1,2,1 });
		blocks[9].SetTranslation({ -1.0f,2.0f,0 });
		blocks[10].SetTranslation({ 5.0f,17.0f,0 });
		blocks[10].SetScale({ 1,4,1 });
		blocks[11].SetScale({ 25,1,1 });
		blocks[11].SetTranslation({ 0,22,0 });
		blocks[12].SetTranslation({ 25.0f,2.0f,0 });
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	}
	for (Block& block : blocks) { block.Initialize(); }
}

void BlockManager::Update()
{
	for (Block& block : blocks) { block.Update(); }
}

void BlockManager::Draw()
{
	for (Block& block : blocks) { block.Draw(); }
}