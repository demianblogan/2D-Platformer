#pragma once

#include <array>
#include <string>

// Campaign progress: which levels are completed and the best star count earned
// on each. Stars: 1 — no deaths, 2 — all fruits collected, 3 — all enemies killed.
class Campaign
{
public:
	static constexpr int LEVEL_COUNT = 20;

	Campaign();

	void Load(const std::string& path);

	// Records a completion, keeps the best star count and saves to disk.
	void RecordCompletion(int levelNumber, int stars);

	bool IsLevelCompleted(int levelNumber) const;
	int GetStars(int levelNumber) const;          // -1 when not completed
	int GetHighestCompletedLevel() const;         // 0 when nothing is completed

	static std::string LevelPath(int levelNumber);
	static bool LevelExists(int levelNumber);

private:
	void Save() const;

	std::string savePath;
	std::array<int, LEVEL_COUNT> bestStars; // -1 = not completed, 0..3 = best stars
};
