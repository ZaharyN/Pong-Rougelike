#pragma once
#include "Types.h"
#include "Paddle.h"
#include <vector>
#include <random>
#include <unordered_set>

class UpgradeManager
{
	std::vector<Upgrade> allUpgrades;
	std::mt19937 rng;
public:
	UpgradeManager();
	std::vector<Upgrade> ChooseThreeRandomUpgrades(const std::unordered_set<UpgradeType>& ownedUniqueUpgrades);

private:
	void LoadUpgradesData();
};