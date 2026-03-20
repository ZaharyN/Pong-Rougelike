#include "UpgradeManager.h"

UpgradeManager::UpgradeManager()
	: rng(std::random_device{}())
{
	LoadUpgradesData();
}

std::vector<Upgrade> UpgradeManager::ChooseThreeRandomUpgrades()
{
	std::vector<Upgrade> selected;
	selected.reserve(3);

	std::discrete_distribution<int> rarityDist({ 60, 30, 10 });

	// Find a way to optimize this because it is O(n^3)
	while (selected.size() < 3)
	{
		int rolledRarityIndex = rarityDist(rng);
		UpgradeRarity targetRarity = static_cast<UpgradeRarity>(rolledRarityIndex);

		std::vector<int> matchingUpgrades;
		for (size_t i = 0; i < allUpgrades.size(); i++)
		{
			if (allUpgrades[i].rarity != targetRarity) continue;

			bool alreadyPicked = false;
			for (const Upgrade& s : selected)
			{
				if (s.title == allUpgrades[i].title)
					alreadyPicked = true;
			}

			if (!alreadyPicked)
				matchingUpgrades.push_back(i);
		}

		if (!matchingUpgrades.empty())
		{
			std::uniform_int_distribution<int> cardDist(0, matchingUpgrades.size() - 1);
			int finalIndex = matchingUpgrades[cardDist(rng)];
			selected.push_back(allUpgrades[finalIndex]);
		}
	}

	return selected;
}

void UpgradeManager::LoadUpgradesData()
{
	// Common upgrades:

	allUpgrades.push_back({
		"SONIC",
		"INCREASSE SPEED BY 10%",
		UpgradeRarity::Common,
		[](Paddle& p, Paddle& opp, Ball& ball)
		{
			p.SetSpeed(1.1f);
		} });

	allUpgrades.push_back({
		"SNAIL",
		"OPPONENT'S SPEED IS REDUCED BY 10%",
		UpgradeRarity::Common,
		[](Paddle& p, Paddle& opp, Ball& ball)
		{
			opp.SetSpeed(0.9f);
		} });

	allUpgrades.push_back({
		"GOLIATH",
		"INCREASE PADDLE WIDTH BY 10%",
		UpgradeRarity::Common,
		[](Paddle& p, Paddle& opp, Ball& ball)
		{
			p.SetSize(1.1f);
		} });

	allUpgrades.push_back({
		"MIDGET",
		"OPPONENT'S PADDLE WIDTH IS REDUCED BY 10%",
		UpgradeRarity::Common,
		[](Paddle& p, Paddle& opp, Ball& ball)
		{
			opp.SetSize(0.9f);
		} });

	allUpgrades.push_back({
		"MORE SPIN",
		"YOUR HITS APPLY 50% MORE SPIN TO THE BALL",
		UpgradeRarity::Common,
		[](Paddle& p, Paddle& opp, Ball& ball)
		{
			p.SetSpin(0.5f);
		} });

	allUpgrades.push_back({
		"LESS SPIN",
		"ENEMIES' HITS APPLY 50% LESS SPIN TO THE BALL",
		UpgradeRarity::Common,
		[](Paddle& p, Paddle& opp, Ball& ball)
		{
			opp.SetSpin(-0.5f);
		} });

	allUpgrades.push_back({
		"NEAR",
		"SPAWN ENERGY RANGE IS REDUCED",
		UpgradeRarity::Common,
		[](Paddle& p, Paddle& opp, Ball& ball)
		{
			p.ModifyEnergySpawnRange(-50.f);
		} });

	allUpgrades.push_back({
		"FAR",
		"OPPONENT'S SPAWN ENERGY RANGE IS INCREASED",
		UpgradeRarity::Common,
		[](Paddle& p, Paddle& opp, Ball& ball)
		{
			opp.ModifyEnergySpawnRange(+50.f);
		} });

	// Uncommon upgrades:

	allUpgrades.push_back({
		"DASH",
		"PRESS SPACE TO APPLY THE DASH",
		UpgradeRarity::Uncommon,
		[](Paddle& p, Paddle& opp, Ball& ball)
		{

		} });

	allUpgrades.push_back({
		"OBSTACLES",
		"OBSTACLES ARE PLACED IN OPPONENTS FIELD",
		UpgradeRarity::Uncommon,
		[](Paddle& p, Paddle& opp, Ball& ball)
		{

		} });

	allUpgrades.push_back({
		"SOMETHING IS MISSING",
		"MAKE A WHOLE IN OPPONENTS BODY",
		UpgradeRarity::Uncommon,
		[](Paddle& p, Paddle& opp, Ball& ball)
		{

		} });

	allUpgrades.push_back({
		"SPING",
		"YOUR HITS APPLY CURVATURE TO THE BALL",
		UpgradeRarity::Uncommon,
		[](Paddle& p, Paddle& opp, Ball& ball)
		{

		} });

	allUpgrades.push_back({
		"FORESIGHT",
		"YOU SEE THE BALL TRAJECTORY",
		UpgradeRarity::Uncommon,
		[](Paddle& p, Paddle& opp, Ball& ball)
		{

		} });

	// Legendary upgrades:

	allUpgrades.push_back({
		"DUMB BUDDY",
		"ADD SECOND RECTANGLE WITH DUMMY MOVEMENT",
		UpgradeRarity::Legendary,
		[](Paddle& p, Paddle& opp, Ball& ball)
		{

		} });

	allUpgrades.push_back({
		"DUPLICATION",
		"SPAWN SECOND BALL ON HIT",
		UpgradeRarity::Legendary,
		[](Paddle& p, Paddle& opp, Ball& ball)
		{

		} });

	allUpgrades.push_back({
		"DUPLICATION",
		"YOU ARE NEVER EXHAUSTED",
		UpgradeRarity::Legendary,
		[](Paddle& p, Paddle& opp, Ball& ball)
		{

		} });

	allUpgrades.push_back({
		"EXTRAVAGANZA",
		"YOU CAN NOW MOVE UP AND DOWN",
		UpgradeRarity::Legendary,
		[](Paddle& p, Paddle& opp, Ball& ball)
		{

		} });
}