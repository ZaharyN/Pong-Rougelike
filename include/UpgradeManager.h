#pragma once
#include "Types.h"
#include <vector>
#include <random>

class UpgradeManager
{
	std::vector<Upgrade> allUpgrades;
	std::mt19937 rng;
public:
	UpgradeManager();
	std::vector<Upgrade> ChooseThreeRandomUpgrades();

private:
	void LoadUpgradesData();
};