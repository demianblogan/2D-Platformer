#pragma once

namespace ECS
{
	struct GroundPatrol
	{
		enum class State { Patrolling, TurningIdle };

		State state      = State::Patrolling;
		int   direction  = -1;
		float speed      = 30.0f;
		float stateTimer = 0.0f;
		bool  paused     = false; // set by AI systems (e.g. TrunkAI) to suspend patrol
		float dustTimer  = 0.0f;  // runtime: countdown to the next run-dust puff

		static constexpr float TURN_IDLE_DURATION = 0.5f;
		static constexpr float DUST_SPACING       = 14.0f; // distance (px) between dust puffs
	};
}
