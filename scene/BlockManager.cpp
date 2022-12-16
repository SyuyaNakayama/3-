#include "BlockManager.h"

BlockManager* BlockManager::GetInstance()
{
    static BlockManager instance;
    return &instance;
}
