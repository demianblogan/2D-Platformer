#include "PatrolSystem.h"

#include "components/Patrol.h"
#include "components/Transform.h"
#include "components/Velocity.h"
#include "core/ecs/Registry.h"

namespace ECS
{
	PatrolSystem::PatrolSystem(Registry& registry)
		: registry(registry)
	{}

	void PatrolSystem::Update()
	{
		registry.ForEach<Patrol, Transform, Velocity>(
			[](Entity, Patrol& patrol, Transform& transform, Velocity& velocity)
			{
				const float position = (patrol.axis == Patrol::Axis::Horizontal) ? transform.x : transform.y;

				if (position <= patrol.min)
					patrol.direction = 1;
				else if (position >= patrol.max)
					patrol.direction = -1;

				const float axisVelocity = patrol.direction * patrol.speed;

				if (patrol.axis == Patrol::Axis::Horizontal)
				{
					velocity.x = axisVelocity;
					velocity.y = 0.0f;
				}
				else
				{
					velocity.x = 0.0f;
					velocity.y = axisVelocity;
				}
			});
	}
}