#pragma once

#include "Th10Ai/Entity.h"

namespace th
{
	class Enemy :
		public Entity
	{
	public:
		bool isBoss() const;
	};
}