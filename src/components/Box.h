#pragma once

#include <string>
#include <vector>

namespace ECS
{
	struct Box
	{
		int hitsToBreak = 1;
		bool dropFruitPerHit = false;
		std::vector<std::string> fruits;

		int hitsTaken = 0;       // runtime
		bool isBreaking = false; // runtime: playing the final Hit, destroyed when it ends
	};
}