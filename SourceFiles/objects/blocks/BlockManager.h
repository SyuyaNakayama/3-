#pragma once
#include "Block.h"
#include <list>
#include <memory>
#include "HideBlock.h"

class BlockManager
{
private:
	enum class BlockType { None, Normal, Move, Copy, Destroy, Ladder, Button, NonCollisionNormal, Goal };

	std::list<std::unique_ptr<BaseBlock>> blocks;
	void LoadMap(const std::string& fileName);
	void CreateBlock(Vector3 pos, Vector3 rot, BlockType type);
	BlockManager() = default;

	HideBlock hideBlock1_;
	HideBlock hideBlock2_;
	HideBlock hideBlock3_;

public:
	BlockManager(const BlockManager& obj) = delete;
	static BlockManager* GetInstance();

	void Initialize();
	void Update();
	void Draw();
	void Clear() { blocks.clear(); }
};