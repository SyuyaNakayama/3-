#pragma once
class BlockManager
{
private:

	BlockManager() = default;
public:
	BlockManager(const BlockManager& obj) = delete;
	static BlockManager* GetInstance();
};