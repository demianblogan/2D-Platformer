#pragma once

namespace sf
{
	class RenderTarget;
}

class Transition
{
public:
	enum class Mode { Reveal, Idle, Cover, Done };

	void StartReveal();
	void StartCover();

	void Update(float deltaTime);
	void Draw(sf::RenderTarget& target);

	Mode GetMode() const;

private:
	float ColumnFillFactor(int column) const; // 0 = no diamond, 1 = full-size diamond

	Mode mode = Mode::Idle;
	float elapsedTime = 0.0f;

	static constexpr int COLUMN_COUNT = 8;
	static constexpr float COLUMN_DELAY = 0.04f;    // stagger between columns
	static constexpr float COLUMN_DURATION = 0.30f; // time for one column to fully shrink or grow
};