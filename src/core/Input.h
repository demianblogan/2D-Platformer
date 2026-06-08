#pragma once

#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <string>
#include <vector>

enum class Action
{
	MoveLeft,
	MoveRight,
	Jump,
	Pause,
	MenuUp,
	MenuDown,
	MenuLeft,
	MenuRight,
	MenuConfirm,
	MenuBack,
	Count
};

enum class InputDevice
{
	Mouse,
	Keyboard,
	Gamepad
};

class Input
{
public:
	void LoadConfig(const std::string& path);
	void LoadDefaults(const std::string& path);

	// Called once per fixed update step, before states update.
	void Update();

	bool IsDown(Action action) const;
	bool WasPressed(Action action) const;
	bool WasReleased(Action action) const;

	float GetAxisX() const; // -1, 0 or +1 (left/right movement)

	InputDevice GetActiveDevice() const { return activeDevice; }

	void NotifyMouseUsed() { activeDevice = InputDevice::Mouse; }

	// Working/saved model for the keyboard rebinding page (mirrors Settings).
	bool IsDirty() const;
	void Revert();
	void SaveConfig(const std::string& path);
	void ResetToDefaults();

	// Keyboard rebinding works on a single primary key per action; gamepad
	// button/axis bindings are left untouched.
	sf::Keyboard::Key GetPrimaryKey(Action action) const;
	void SetPrimaryKey(Action action, sf::Keyboard::Key key);

	static std::string KeyName(sf::Keyboard::Key key);

private:
	enum class BindingType
	{
		Key,
		Button,
		Axis
	};

	struct Binding
	{
		BindingType type = BindingType::Key;
		sf::Keyboard::Key key = sf::Keyboard::Key::Unknown;
		unsigned int button = 0;
		sf::Joystick::Axis axis = sf::Joystick::Axis::X;
		float direction = 0.0f; // sign for an axis binding

		bool operator==(const Binding& other) const = default;
	};

	static constexpr int ACTION_COUNT = static_cast<int>(Action::Count);

	using BindingSet = std::vector<Binding>[ACTION_COUNT];

	static void LoadBindingsFile(const std::string& path, BindingSet target, float* outAxisThreshold);

	static int FindGamepad();
	bool IsBindingDown(const Binding& binding, int gamepad, bool& fromGamepad) const;

	std::vector<Binding> bindings[ACTION_COUNT];        // working set used by Update
	std::vector<Binding> savedBindings[ACTION_COUNT];   // last persisted state
	std::vector<Binding> defaultBindings[ACTION_COUNT]; // factory defaults

	bool currentDown[ACTION_COUNT] = {};
	bool previousDown[ACTION_COUNT] = {};

	float axisThreshold = 50.0f; // percent of full axis travel
	InputDevice activeDevice = InputDevice::Keyboard;
};