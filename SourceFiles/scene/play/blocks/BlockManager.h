#pragma once
#include "Block.h"
#include <list>
#include <memory>

class BlockManager
{
private:
	enum class BlockType { Copy, Move, Destroy };

	std::list<std::unique_ptr<BaseBlock>> blocks;

	BlockManager() = default;
	void CreateBlock(Vector3 pos, Vector3 scale, BlockType type);
public:
	BlockManager(const BlockManager& obj) = delete;
	static BlockManager* GetInstance();

	void Initialize(UINT16 stage);
	void Update();
	void Draw();
};