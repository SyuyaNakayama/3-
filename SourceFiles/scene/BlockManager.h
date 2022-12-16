#pragma once
#include "Block.h"
#include <vector>

class BlockManager
{
private:
	std::vector<Block> blocks;

	BlockManager() = default;
public:
	BlockManager(const BlockManager& obj) = delete;
	static BlockManager* GetInstance();

	void Initialize(UINT16 stage);
	void Update();
	void Draw();
};