#pragma once

namespace ECS
{
	class Registry;

	class BoxSystem
	{
	public:
		BoxSystem(Registry& registry);

		void Update();

	private:
		Registry& registry;
	};
}