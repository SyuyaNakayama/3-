#pragma once
#include "Block.h"
#include <vector>
#include <memory>

class BlockManager
{
private:
	std::vector<std::unique_ptr<Block>> blocks;

	BlockManager() = default;
	void CreateBlock(Vector3 pos, Vector3 scale);
public:
	BlockManager(const BlockManager& obj) = delete;
	static BlockManager* GetInstance();

	void Initialize(UINT16 stage);
	void Update();
	void Draw();
};