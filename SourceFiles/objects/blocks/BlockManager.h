#pragma once
#include "Block.h"
#include <list>
#include <array>
#include <memory>

class BlockManager
{
private:
	enum class BlockType { None, Normal, Move, Copy, Destroy, Ladder, Button, NonCollisionNormal, Goal };
	static const UINT16 STAGE_SIZE = 40;

	std::list<std::unique_ptr<BaseBlock>> blocks;
	void LoadMap(const std::string& fileName, UINT16 faceNum);
	void CreateBlock(Vector3 pos, Vector3 rot, BlockType type);
	BlockManager() = default;
public:
	BlockManager(const BlockManager& obj) = delete;
	static BlockManager* GetInstance();

	void Initialize(UINT16 stage);
	void Update();
	void Draw();
	void Clear() { blocks.clear(); }
};