#pragma once
#include "Block.h"
#include <list>
#include <memory>

class BlockManager
{
private:
	enum class BlockType { None, Normal, Move, Copy, Destroy, Ladder, Button, NonCollisionNormal, Goal };

	std::list<std::unique_ptr<BaseBlock>> blocks;
	void LoadMap(const std::string& fileName);
	void CreateBlock(Vector3 pos, Vector3 rot, BlockType type);
	BlockManager() = default;
public:
	BlockManager(const BlockManager& obj) = delete;
	static BlockManager* GetInstance();

	void Initialize();
	void Update();
	void Draw();
	void SpriteDraw() { for (const std::unique_ptr<BaseBlock>& block : blocks) { block->SpriteDraw(); } };
	void Clear() { blocks.clear(); }
};