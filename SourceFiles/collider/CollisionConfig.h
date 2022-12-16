#pragma once

namespace CollisionAttribute
{
	enum
	{
		Player = 0b1,
		Enemy = 0b1 << 1,
	};
}

namespace CollisionMask
{
	enum
	{
		Player = ~CollisionAttribute::Player,
		Enemy = ~CollisionAttribute::Enemy,
	};
}