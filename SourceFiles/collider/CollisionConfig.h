#pragma once

enum class CollisionAttribute
{
	Player = 0b1,
	Blocks = 0b1 << 1,
};

enum class CollisionMask
{
	Player = ~(int)CollisionAttribute::Player,
	Blocks = ~(int)CollisionAttribute::Blocks,
};