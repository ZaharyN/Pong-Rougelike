#pragma once
#include "Types.h"
#include "Paddle.h"
#include "Buddy.h"
#include <vector>
#include <random>
#include <unordered_set>

class UpgradeManager
{
	static constexpr float SONIC_FACTOR = 1.1f;
	static constexpr float SNAIL_FACTOR = 0.9f;
	static constexpr float GOLIATH_FACTOR = 1.1f;
	static constexpr float MIDGET_FACTOR = 0.9f;
	static constexpr float MORE_SPIN_FACTOR = 1.5f;
	static constexpr float LESS_SPIN_FACTOR = 0.5f;
	static constexpr float NEAR_FACTOR = -50.f;
	static constexpr float FAR_FACTOR = 50.f;
	static constexpr float OBSTACLE_WIDTH = 50.f;
	static constexpr float OBSTACLE_HEIGHT = 10.f;
	static constexpr float CURVATURE_POWER = 0.5f;
	static constexpr float BUDDY_WIDTH = 80.f;
	static constexpr float BUDDY_HEIGHT = 10.f;

	const unsigned int WINDOW_WIDTH;
	const unsigned int WINDOW_HEIGHT;

	std::vector<Upgrade> allUpgrades;
	std::mt19937 rng;
public:
	UpgradeManager(const unsigned int windowWidth, const unsigned int windowHeight);
	std::vector<Upgrade> ChooseThreeRandomUpgrades(const std::unordered_set<UpgradeType>& ownedUniqueUpgrades);

private:
	void LoadUpgradesData();
};