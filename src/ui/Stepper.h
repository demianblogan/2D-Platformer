#pragma once

#include "ui/InteractiveElement.h"

#include <SFML/Graphics/Color.hpp>

#include <array>
#include <functional>
#include <memory>

namespace UI
{
	// A "left value right" control. Like a slider, it must be activated before the
	// keyboard left/right step it; the arrow images stay clickable with the mouse.
	class Stepper : public InteractiveElement
	{
	public:
		void SetLeftArrow(std::unique_ptr<Element> element);
		void SetRightArrow(std::unique_ptr<Element> element);
		void SetValueLabel(std::unique_ptr<Element> element);

		void SetArrowColor(InteractionState state, sf::Color color);
		void SetValueColor(sf::Color color);

		void SetOnStepLeft(std::function<void()> callback);
		void SetOnStepRight(std::function<void()> callback);

		bool RequiresActivation() const override { return true; }

		void OnDragStart(sf::Vector2f mousePosition) override;
		void OnDragEnd() override;
		void OnNavigate(int direction) override;

	protected:
		void OnStateChanged() override;
		void OnEnabledChanged() override;

	private:
		enum class PressedArrow
		{
			None,
			Left,
			Right
		};

		void RefreshColors();
		sf::Color ColorForArrow(PressedArrow arrow) const;

		Element* leftArrow = nullptr;
		Element* rightArrow = nullptr;
		Element* valueLabel = nullptr;

		std::array<sf::Color, INTERACTION_STATE_COUNT> arrowColors =
		{
			sf::Color::White,
			sf::Color::White,
			sf::Color::White
		};
		sf::Color valueColor = sf::Color::White;

		PressedArrow pressedArrow = PressedArrow::None;

		std::function<void()> onStepLeft;
		std::function<void()> onStepRight;
	};
}