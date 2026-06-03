#include "SplashState.h"

#include "Context.h"
#include "audio/Mixer.h"
#include "core/Resources.h"
#include "core/StateMachine.h"
#include "core/VirtualScreen.h"
#include "states/TestState.h"
#include "tilemap/TilemapLoader.h"
#include "tilemap/TilemapRenderer.h"
#include "ui/Animation.h"
#include "ui/Label.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <memory>
#include <stdexcept>

SplashState::SplashState(Context& context)
	: State(context)
	, patrolSystem(registry)
	, movementSystem(registry)
	, animationSystem(registry)
	, renderSystem(registry, context.resources, context.virtualScreen.GetRenderTarget())
	, background(context.resources)
	, userInterface(context.virtualScreen)
	, interfaceLoader(context.resources)
{
	Resources& resources = context.resources;

	resources.fonts.Load("main", "assets/fonts/main.ttf");
	resources.fonts.Get("main").setSmooth(false);
	resources.LoadTexturesFromFile("data/levels/splash_textures.json");

	background.SetTexture("green");
	background.SetDirection(AnimatedBackground::Direction::Down);
	background.SetSpeed(16.0f);

	tilemap = LoadTilemap("data/levels/splash_level_tilemap.tmj", "terrain", 22);

	const sf::Vector2f worldSize =
	{
		static_cast<float>(tilemap.GetWidth() * tilemap.tileSize),
		static_cast<float>(tilemap.GetHeight() * tilemap.tileSize)
	};
	camera.SetWorldSize(worldSize);

	const float halfViewWidth = VirtualScreen::WIDTH / 2.0f;
	panMinX = halfViewWidth;
	panMaxX = worldSize.x - halfViewWidth;
	panX = panMinX;
	panY = worldSize.y / 2.0f;
	camera.SnapTo({ panX, panY });

	sceneLoader.LoadSceneFromMap(registry, "data/levels/splash_level_tilemap.tmj");

	BuildInterface();

	context.audioMixer.PlayMusic("menu_theme");

	transition.StartReveal();
}

void SplashState::BuildInterface()
{
	userInterface.SetContent(interfaceLoader.LoadFromFile("data/ui/splash.json"));

	UI::Element* title = userInterface.FindByName("title");
	UI::Label* prompt = dynamic_cast<UI::Label*>(userInterface.FindByName("prompt"));

	if (title == nullptr || prompt == nullptr)
		throw std::runtime_error("SplashState: splash.json must contain 'title' and 'prompt'");

	const float titleRestY = title->offset.y;
	const float titleStartY = -title->size.y - 10.0f;
	title->offset.y = titleStartY;

	prompt->isVisible = false;

	UI::Animation& slide = title->AddAnimation(std::make_unique<UI::Animation>(
		titleStartY, titleRestY, 1.6f,
		UI::AnimationCurve::EaseOut, UI::AnimationLoop::Once,
		[title](float y) { title->offset.y = y; }));

	slide.SetOnFinished([prompt]()
		{
			prompt->isVisible = true;
			prompt->AddAnimation(std::make_unique<UI::Animation>(
				1.0f, 0.25f, 0.6f,
				UI::AnimationCurve::Sine, UI::AnimationLoop::PingPong,
				[prompt](float alpha) { prompt->SetAlpha(alpha); }));
		});
}

void SplashState::HandleEvent(const sf::Event& event)
{
	// Accept input only once the intro is fully revealed.
	if (transition.GetMode() != Transition::Mode::Idle)
		return;

	const bool anyInput =
		event.is<sf::Event::KeyPressed>() ||
		event.is<sf::Event::MouseButtonPressed>() ||
		event.is<sf::Event::JoystickButtonPressed>();

	if (anyInput)
		transition.StartCover();
}

void SplashState::Update(float deltaTime)
{
	transition.Update(deltaTime);

	if (transition.GetMode() != Transition::Mode::Done)
	{
		panX += panDirection * PAN_SPEED * deltaTime;

		if (panX >= panMaxX)
		{
			panX = panMaxX;
			panDirection = -1;
		}
		else if (panX <= panMinX)
		{
			panX = panMinX;
			panDirection = 1;
		}

		camera.MoveTo({ panX, panY });
	}

	background.Update(deltaTime);

	patrolSystem.Update();
	movementSystem.Update(deltaTime);
	animationSystem.Update(deltaTime);

	userInterface.Update(deltaTime);

	if (transition.GetMode() == Transition::Mode::Done && !isLeaving)
	{
		isLeaving = true;
		context.stateMachine.Clear();
		context.stateMachine.Push(std::make_unique<TestState>(context));
	}
}

void SplashState::Render(float interpolationFactor)
{
	sf::RenderTarget& renderTarget = context.virtualScreen.GetRenderTarget();

	renderTarget.clear(sf::Color::Black);

	// Background: screen space, behind everything.
	context.virtualScreen.SetCameraCenter(VirtualScreen::WIDTH / 2.0f, VirtualScreen::HEIGHT / 2.0f);
	background.Draw(renderTarget);

	// World: camera space.
	const sf::Vector2f worldCenter = camera.GetRenderCenter(interpolationFactor);
	context.virtualScreen.SetCameraCenter(worldCenter.x, worldCenter.y);

	DrawTilemap(tilemap, renderTarget, context.resources);
	renderSystem.Render(interpolationFactor);

	// UI and transition: screen space, on top.
	context.virtualScreen.SetCameraCenter(VirtualScreen::WIDTH / 2.0f, VirtualScreen::HEIGHT / 2.0f);
	userInterface.Draw(renderTarget);
	transition.Draw(renderTarget);
}