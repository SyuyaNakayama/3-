#include <imgui.h>
#include <fstream>
#include <sstream>
#include <array>
#include "BlockManager.h"
#include "Quaternion.h"
#include "ImGuiManager.h"
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
	case 0: LoadMap("title.txt", 0); break;
	case 1: LoadMap("tutorial.txt", 1); break;
	case 2: LoadMap("stageDeb.txt", 2); break;
		//case 2: LoadMap("tutorial.txt", 2); break;
	case 3: LoadMap("stage2.txt", 3); break;
	case 4: LoadMap("stage3.txt", 4); break;
	case 5: LoadMap("clear.txt", 5); break;
	}
	StagePlane::GetInstance()->Initialize();
	unique_ptr<BaseBlock> bgBlock = make_unique<BgBlock>();
	bgBlock->Initialize();
	blocks.push_back(move(bgBlock));

	//array<Vector3, 3> stopBlock =
	//{ {
	//	{ -20,-20,-40 }, //����
	//	{ -20, 20,-40 }, //����
	//	{  20, 20,-40 }  //�E��
	//} };

	//for (size_t i = 0; i < stopBlock.size(); i++)
	//{
	//	unique_ptr<BaseBlock> StopBlock_ = make_unique<StopBlock>();
	//	StopBlock_->SetTranslation(RotateVector(stopBlock[i], CubeQuaternion::Get(stage)));
	//	StopBlock_->SetScale({ 20,20,1 });
	//	StopBlock_->SetNum(i + 1);
	//	StopBlock_->Initialize();
	//	blocks.push_back(move(StopBlock_));
	//}
}

void BlockManager::Update()
{
	// �j�󂳂ꂽ�u���b�N�̔r��
	blocks.remove_if([](const unique_ptr<BaseBlock>& block) { return block->IsDestroy(); });

	// �R�s�[���ꂽ�u���b�N�̒ǉ�
	unique_ptr<BaseBlock> newBlock;
	for (const unique_ptr<BaseBlock>& block : blocks)
	{
		if (newBlock = block->NewBlockCreate())
		{
			blocks.push_back(move(newBlock));
			break;
		}
	}

	for (const unique_ptr<BaseBlock>& block : blocks) { block->Update(); }
}

void BlockManager::Draw() { for (const unique_ptr<BaseBlock>& block : blocks) { block->Draw(); } }

void BlockManager::LoadMap(const std::string& fileName, UINT16 faceNum)
{
	fstream file;
	file.open("Resources/stages/" + fileName);
	assert(file.is_open());

	string line, key;
	Quaternion rotQ = CubeQuaternion::Get(faceNum);
	for (UINT16 y = 0; y < STAGE_SIZE;)
	{
		getline(file, line);
		if (line.empty()) { continue; }
		istringstream lineStream(line);

		for (UINT16 x = 0; x < STAGE_SIZE; x++)
		{
			int temp;
			lineStream >> temp;
			Vector3 pos = { 2.0f * x - 39.0f,-2.0f * y + 39.0f,-39.0f };
			Vector3 rotVec{};
			if (temp == (int)BlockType::Button) { rotVec = RotateVector(Vector3(-PI / 2.0f, 0, 0), rotQ); }
			if (temp == (int)BlockType::Ladder) { rotVec.y = PI - PI / 2.0f * (float)(faceNum - 1); }
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