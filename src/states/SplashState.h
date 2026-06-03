#pragma once

#include "core/Camera.h"
#include "core/DataLoader.h"
#include "core/State.h"
#include "core/ecs/Registry.h"
#include "graphics/AnimatedBackground.h"
#include "graphics/Transition.h"
#include "systems/AnimationSystem.h"
#include "systems/MovementSystem.h"
#include "systems/PatrolSystem.h"
#include "systems/RenderSystem.h"
#include "tilemap/Tilemap.h"
#include "ui/DataLoader.h"
#include "ui/Root.h"

#include <SFML/System/Vector2.hpp>

class SplashState : public State
{
public:
	SplashState(Context& context);

	void HandleEvent(const sf::Event& event) override;
	void Update(float deltaTime) override;
	void Render(float interpolationFactor) override;

private:
	void BuildInterface();

	ECS::Registry registry;
	DataLoader sceneLoader;

	ECS::PatrolSystem patrolSystem;
	ECS::MovementSystem movementSystem;
	ECS::AnimationSystem animationSystem;
	ECS::RenderSystem renderSystem;

	Camera camera;
	Tilemap tilemap;
	AnimatedBackground background;

	UI::Root userInterface;
	UI::DataLoader interfaceLoader;

	Transition transition;

	float panX = 0.0f;
	float panY = 0.0f;
	float panMinX = 0.0f;
	float panMaxX = 0.0f;
	int panDirection = 1;

	bool isLeaving = false;

	static constexpr float PAN_SPEED = 60.0f; // pixels per second (1 virtual pixel per frame at 60 FPS)
};