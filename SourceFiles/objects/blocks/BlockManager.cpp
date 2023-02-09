#include <fstream>
#include <sstream>
#include <array>
#include <cassert>
#include "GameScene.h"
#include "BlockManager.h"
#include "Quaternion.h"

using namespace std;



BlockManager* BlockManager::GetInstance()
{
	static BlockManager instance;
	return &instance;
}

void BlockManager::Initialize()
{
	Clear();
	switch (*GameScene::GetStage())
	{
	case 0: LoadMap("title.txt"); break;
	case 1: LoadMap("tutorial.txt"); break;
	case 2: LoadMap("stage1.txt"); break;
	case 3: LoadMap("stage2.txt"); break;
	case 4: LoadMap("stage3.txt"); break;
	case 5: LoadMap("clear.txt"); break;
	}

	StagePlane::GetInstance()->Initialize();
	unique_ptr<BaseBlock> bgBlock = make_unique<BgBlock>();
	bgBlock->Initialize();
	blocks.push_back(move(bgBlock));

	array<Vector3, 3> stopBlock =
	{ {
		{ -20,-20,-40 }, //左下
		{ -20, 20,-40 }, //左上
		{  20, 20,-40 }  //右上
	} };

	array<Vector3, 2> stopBlockScale =
	{ {
		{20,20,1},{1,20,20}
	} };

	for (size_t i = 0; i < stopBlock.size(); i++)
	{
		unique_ptr<BaseBlock> StopBlock_ = make_unique<StopBlock>();
		StopBlock_->SetTranslation(RotateVector(stopBlock[i], CubeQuaternion::Get()));
		StopBlock_->SetScale(stopBlockScale[*GameScene::GetStage() % 2 == 0]);
		StopBlock_->SetNum(i + 1);
		StopBlock_->Initialize();
		blocks.push_back(move(StopBlock_));
	}

	hideBlock1_.Initialize();
	hideBlock2_.Initialize();
	hideBlock3_.Initialize();
	switch (*GameScene::GetStage())
	{
	case 1://チュートリアル　手前
		hideBlock1_.SetTransfer({ -20, -20, -40.5 }, { 20,20,1 });
		hideBlock2_.SetTransfer({ -20,20,-40.5 }, { 20,20,1 });
		hideBlock3_.SetTransfer({ 20,20,-40.5 }, { 20,20,1 });
		break;
	case 2://ステージ1 右
		hideBlock1_.SetTransfer({ 40.5, -20, -20 }, { 1,20,20 });
		hideBlock2_.SetTransfer({ 40.5,20,-20 }, { 1,20,20 });
		hideBlock3_.SetTransfer({ 40.5,20,20 }, { 1,20,20 });
		break;
	case 3://ステージ2　奥
		hideBlock1_.SetTransfer({ 20, -20, 40.5 }, { 20,20,1 });
		hideBlock2_.SetTransfer({ 20,20,40.5 }, { 20,20,1 });
		hideBlock3_.SetTransfer({ -20,20,40.5 }, { 20,20,1 });
		break;
	case 4://ステージ3　左
		hideBlock1_.SetTransfer({ -40.5,-20, 20 }, { 1,20,20 });
		hideBlock2_.SetTransfer({ -40.5,20,20 }, { 1,20,20 });
		hideBlock3_.SetTransfer({ -40.5,20,-20 }, { 1,20,20 });
		break;
	}

	MoveBlock::isExplanation = false;
	CopyBlock::isExplanation = false;
}

void BlockManager::Update()
{
	// 破壊されたブロックの排除
	blocks.remove_if([](const unique_ptr<BaseBlock>& block) { return block->IsDestroy(); });

	// コピーされたブロックの追加
	unique_ptr<BaseBlock> newBlock;
	for (const unique_ptr<BaseBlock>& block : blocks)
	{
		if (newBlock = block->NewBlockCreate())
		{
			blocks.push_back(move(newBlock));
			break;
		}
	}
	hideBlock1_.Update(Button::GetUseCount(), 1);
	hideBlock2_.Update(Button::GetUseCount(), 2);
	hideBlock3_.Update(Button::GetUseCount(), 3);
	

	for (const unique_ptr<BaseBlock>& block : blocks) { block->Update(); }
}

void BlockManager::Draw() 
{ 
	for (const unique_ptr<BaseBlock>& block : blocks) 
	{ 
		block->Draw();
	} 
	hideBlock1_.Draw();
	hideBlock2_.Draw();
	hideBlock3_.Draw();
}

void BlockManager::LoadMap(const std::string& fileName)
{
	fstream file;
	file.open("Resources/stages/" + fileName);
	assert(file.is_open());

	string line, key;
	Quaternion rotQ = CubeQuaternion::Get();
	for (UINT16 y = 0; y < 40;)
	{
		getline(file, line);
		if (line.empty()) { continue; }
		istringstream lineStream(line);

		for (UINT16 x = 0; x < 40; x++)
		{
			int temp;
			lineStream >> temp;
			Vector3 pos = { 2.0f * x - 39.0f,-2.0f * y + 39.0f,-39.0f };
			Vector3 rotVec{};
			if (temp == (int)BlockType::Button) { rotVec = RotateVector(Vector3(-PI / 2.0f, 0, 0), rotQ); }
			if (temp == (int)BlockType::Ladder) { rotVec.y = PI - PI / 2.0f * (float)(*GameScene::GetStage() - 1); }
			CreateBlock(RotateVector(pos, rotQ), rotVec, (BlockType)temp);
			getline(lineStream, key, ',');
		}
		y++;
	}
	file.close();
}

void BlockManager::CreateBlock(Vector3 pos, Vector3 rot, BlockType type)
{
	unique_ptr<BaseBlock> block;
	switch (type)
	{
	case BlockType::None:																return;
	case BlockType::Normal:				block = make_unique<NormalBlock>();				break;
	case BlockType::Move:				block = make_unique<MoveBlock>();				break;
	case BlockType::Copy:				block = make_unique<CopyBlock>();				break;
	case BlockType::Destroy:			block = make_unique<DestroyBlock>();			break;
	case BlockType::Ladder:				block = make_unique<LadderBlock>();				break;
	case BlockType::Button:				block = make_unique<Button>();					break;
	case BlockType::NonCollisionNormal:	block = make_unique<NonCollisionNormalBlock>();	break;
	case BlockType::Goal:				block = make_unique<GoalBlock>();				break;
	}
	assert(block);
	block->SetTranslation(pos);
	block->SetRotation(rot);
	block->Initialize();
	blocks.push_back(move(block));
}