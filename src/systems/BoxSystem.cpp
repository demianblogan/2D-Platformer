#include "BoxSystem.h"

#include "components/Animation.h"
#include "components/AnimationState.h"
#include "components/Box.h"
#include "components/BoxHit.h"
#include "core/ecs/Registry.h"

#include <vector>

namespace ECS
{
	BoxSystem::BoxSystem(Registry& registry)
		: registry(registry)
	{}

	void BoxSystem::Update()
	{
		// Apply hits taken this frame.
		std::vector<Entity> hits;
		registry.ForEach<Box, BoxHit>(
			[&hits](Entity entity, Box&, BoxHit&) { hits.push_back(entity); });

		for (const Entity entity : hits)
		{
			Box& box = registry.Get<Box>(entity);

			if (!box.isBreaking) // ignore extra bounces once it's already breaking
			{
				box.hitsTaken++;

				if (registry.Has<AnimationState>(entity))
					registry.Get<AnimationState>(entity).current = "Hit";

				if (box.hitsTaken >= box.hitsToBreak)
					box.isBreaking = true;

				// (fruit ejection comes in Part 3)
			}

			registry.RemoveFrom<BoxHit>(entity);
		}

		// When the Hit animation finishes: break (if breaking) or return to Idle.
		std::vector<Entity> toBreak;

		registry.ForEach<Box, AnimationState, Animation>(
			[&toBreak](Entity entity, Box& box, AnimationState& state, Animation& animation)
			{
				if (state.current == "Hit" && !animation.data.isLooping
					&& animation.currentFrame >= animation.data.frameCount - 1)
				{
					if (box.isBreaking)
						toBreak.push_back(entity);
					else
						state.current = "Idle";
				}
			});

		for (const Entity entity : toBreak)
			registry.DestroyEntity(entity); // (fruit + debris on break come in Part 3)
	}
}