#include "Stepper.h"

#include <SFML/Graphics/Rect.hpp>

namespace UI
{
	namespace
	{
		int StateToIndex(InteractionState state)
		{
			switch (state)
			{
			case InteractionState::Normal:
				return 0;
			case InteractionState::Highlighted:
				return 1;
			case InteractionState::Pressed:
				return 2;
			default:
				return 0;
			}
		}
	}

	void Stepper::SetLeftArrow(std::unique_ptr<Element> element)
	{
		Element& added = AddChild(std::move(element));
		leftArrow = &added;
		RefreshColors();
	}

	void Stepper::SetRightArrow(std::unique_ptr<Element> element)
	{
		Element& added = AddChild(std::move(element));
		rightArrow = &added;
		RefreshColors();
	}

	void Stepper::SetValueLabel(std::unique_ptr<Element> element)
	{
		Element& added = AddChild(std::move(element));
		valueLabel = &added;
		RefreshColors();
	}

	void Stepper::SetArrowColor(InteractionState state, sf::Color color)
	{
		arrowColors[StateToIndex(state)] = color;
		RefreshColors();
	}

	void Stepper::SetValueColor(sf::Color color)
	{
		valueColor = color;
		RefreshColors();
	}

	void Stepper::SetOnStepLeft(std::function<void()> callback)
	{
		onStepLeft = std::move(callback);
	}

	void Stepper::SetOnStepRight(std::function<void()> callback)
	{
		onStepRight = std::move(callback);
	}

	void Stepper::OnDragStart(sf::Vector2f mousePosition)
	{
		if (!isEnabled)
			return;

		if (leftArrow != nullptr)
		{
			const sf::FloatRect bounds(leftArrow->GetAbsolutePosition(), leftArrow->size);
			if (bounds.contains(mousePosition))
			{
				pressedArrow = PressedArrow::Left;
				RefreshColors();

				if (onStepLeft)
					onStepLeft();

				return;
			}
		}

		if (rightArrow != nullptr)
		{
			const sf::FloatRect bounds(rightArrow->GetAbsolutePosition(), rightArrow->size);
			if (bounds.contains(mousePosition))
			{
				pressedArrow = PressedArrow::Right;
				RefreshColors();

				if (onStepRight)
					onStepRight();

				return;
			}
		}
	}

	void Stepper::OnDragEnd()
	{
		pressedArrow = PressedArrow::None;
		RefreshColors();
	}

	void Stepper::OnNavigate(int direction)
	{
		if (!isEnabled)
			return;

		if (direction < 0)
		{
			if (onStepLeft)
				onStepLeft();
		}
		else if (direction > 0)
		{
			if (onStepRight)
				onStepRight();
		}
	}

	void Stepper::OnStateChanged()
	{
		RefreshColors();
	}

	void Stepper::OnEnabledChanged()
	{
		RefreshColors();
	}

	void Stepper::RefreshColors()
	{
		if (leftArrow != nullptr)
			leftArrow->SetColor(ColorForArrow(PressedArrow::Left));
		if (rightArrow != nullptr)
			rightArrow->SetColor(ColorForArrow(PressedArrow::Right));
		if (valueLabel != nullptr)
			valueLabel->SetColor(isEnabled ? valueColor : disabledColor);
	}

	sf::Color Stepper::ColorForArrow(PressedArrow arrow) const
	{
		if (!isEnabled)
			return disabledColor;

		if (pressedArrow == arrow)
			return arrowColors[StateToIndex(InteractionState::Pressed)];

		return arrowColors[StateToIndex(state)];
	}
}