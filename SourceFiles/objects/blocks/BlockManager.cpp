#include "BlockManager.h"
#include <imgui.h>
#include <fstream>
#include <sstream>
#include "Quaternion.h"
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
	//case 2: LoadMap("stageDeb.txt", 2); break;
	case 2: LoadMap("tutorial.txt", 2); break;
	case 3: LoadMap("stage2.txt", 3); break;
	case 4: LoadMap("stage3.txt", 4); break;
	case 5: LoadMap("clear.txt", 5); break;
	}
	StagePlane::GetInstance()->Initialize();
	unique_ptr<BaseBlock> bgBlock = make_unique<BgBlock>();
	bgBlock->Initialize();
	blocks.push_back(move(bgBlock));

	//左下
	unique_ptr<BaseBlock> StopBlock_ = make_unique<StopBlock>();

	float scale = 21.0f;
	StopBlock_->SetTranslation({ -19,-19,-40 });
	StopBlock_->SetScale({ scale,scale,1 });
	StopBlock_->SetNum(1);
	StopBlock_->Initialize();
	blocks.push_back(move(StopBlock_));

	//左上
	StopBlock_ = make_unique<StopBlock>();
	StopBlock_->SetTranslation({ -19,19,-40 });
	StopBlock_->SetScale({ scale,scale,1 });
	StopBlock_->SetNum(2);
	StopBlock_->Initialize();
	blocks.push_back(move(StopBlock_));

	//右上
	StopBlock_ = make_unique<StopBlock>();
	StopBlock_->SetTranslation({ 19,19,-40 });
	StopBlock_->SetScale({ scale,scale,1 });
	StopBlock_->SetNum(3);
	StopBlock_->Initialize();
	blocks.push_back(move(StopBlock_));
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

	for (const unique_ptr<BaseBlock>& block : blocks) { block->Update(); }

	ImGui::Text("count:%d", Button::useCount);
}

void BlockManager::Draw()
{
	uint16_t num;
	for (const unique_ptr<BaseBlock>& block : blocks)
	{
		uint16_t num = block->GetNum();
		if (num == 0) { block->Draw(); }
	}
}

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
			if (temp == (int)BlockType::Button) { rotVec = Quaternion::RotateVector(Vector3(-PI / 2.0f, 0, 0), rotQ); }
			if (temp == (int)BlockType::Ladder) { rotVec.y = PI - PI / 2.0f * (float)(faceNum - 1); }
			CreateBlock(Quaternion::RotateVector(pos, rotQ), rotVec, (BlockType)temp);
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