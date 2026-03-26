#include "UpgradeManager.h"

UpgradeManager::UpgradeManager()
	: rng(std::random_device{}())
{
	LoadUpgradesData();
}

std::vector<Upgrade> UpgradeManager::ChooseThreeRandomUpgrades(const std::unordered_set<UpgradeType>& ownedUniqueUpgrades)
{
	std::vector<Upgrade> selected;
	selected.reserve(3);

	std::discrete_distribution<int> rarityDist({ 60, 30, 10 });

	while (selected.size() < 3)
	{
		int rolledRarityIndex = rarityDist(rng);
		UpgradeRarity targetRarity = static_cast<UpgradeRarity>(rolledRarityIndex);

		std::vector<int> matchingUpgrades;
		for (size_t i = 0; i < allUpgrades.size(); i++)
		{
			if (allUpgrades[i].rarity != targetRarity) continue;
			if (ownedUniqueUpgrades.find(allUpgrades[i].type) != ownedUniqueUpgrades.end())
				continue;

			bool alreadyPicked = false;
			for (const Upgrade& s : selected)
			{
				if (s.type == allUpgrades[i].type)
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
		UpgradeType::Sonic,
		"SONIC",
		"INCREASE SPEED BY 10%",
		UpgradeRarity::Common,
		false,
		[](Paddle& p, Paddle& opp, Ball& ball)
		{
			p.AddUpgrade(UpgradeType::Sonic, false);
			p.SetSpeed(1.1f);
		} });

	allUpgrades.push_back({
		UpgradeType::Snail,
		"SNAIL",
		"OPPONENT'S SPEED IS REDUCED BY 10%",
		UpgradeRarity::Common,
		false,
		[](Paddle& p, Paddle& opp, Ball& ball)
		{
			opp.AddUpgrade(UpgradeType::Snail, false);
			opp.SetSpeed(0.9f);
		} });

	allUpgrades.push_back({
		UpgradeType::Goliath,
		"GOLIATH",
		"INCREASE PADDLE WIDTH BY 10%",
		UpgradeRarity::Common,
		false,
		[](Paddle& p, Paddle& opp, Ball& ball)
		{
			p.AddUpgrade(UpgradeType::Goliath, false);
			p.SetSize(1.1f);
		} });

	allUpgrades.push_back({
		UpgradeType::Midget,
		"MIDGET",
		"OPPONENT'S PADDLE WIDTH IS REDUCED BY 10%",
		UpgradeRarity::Common,
		false,
		[](Paddle& p, Paddle& opp, Ball& ball)
		{
			opp.AddUpgrade(UpgradeType::Midget, false);
			opp.SetSize(0.9f);
		} });

	allUpgrades.push_back({
		UpgradeType::MoreSpin,
		"MORE SPIN",
		"YOUR HITS APPLY 50% MORE SPIN TO THE BALL",
		UpgradeRarity::Common,
		false,
		[](Paddle& p, Paddle& opp, Ball& ball)
		{
			p.AddUpgrade(UpgradeType::MoreSpin, false);
			p.SetSpin(1.5f);
		} });

	allUpgrades.push_back({
		UpgradeType::LessSpin,
		"LESS SPIN",
		"ENEMIES' HITS APPLY 50% LESS SPIN TO THE BALL",
		UpgradeRarity::Common,
		false,
		[](Paddle& p, Paddle& opp, Ball& ball)
		{
			opp.AddUpgrade(UpgradeType::LessSpin, false);
			opp.SetSpin(0.5f);
		} });

	allUpgrades.push_back({
		UpgradeType::Near,
		"NEAR",
		"SPAWN ENERGY RANGE IS REDUCED",
		UpgradeRarity::Common,
		false,
		[](Paddle& p, Paddle& opp, Ball& ball)
		{
			p.AddUpgrade(UpgradeType::Near, false);
			p.ModifyEnergySpawnRange(-50.f);
		} });

	allUpgrades.push_back({
		UpgradeType::Far,
		"FAR",
		"OPPONENT'S SPAWN ENERGY RANGE IS INCREASED",
		UpgradeRarity::Common,
		false,
		[](Paddle& p, Paddle& opp, Ball& ball)
		{
			opp.AddUpgrade(UpgradeType::Far, false);
			opp.ModifyEnergySpawnRange(+50.f);
		} });

	// Uncommon upgrades:

	allUpgrades.push_back({
		UpgradeType::Dash,
		"DASH",
		"PRESS SPACE TO APPLY THE DASH",
		UpgradeRarity::Uncommon,
		true,
		[](Paddle& p, Paddle& opp, Ball& ball)
		{
			p.AddUpgrade(UpgradeType::Dash, true);
			p.EnableDash();
		} });

	allUpgrades.push_back({
		UpgradeType::Obstacles,
		"OBSTACLES",
		"OBSTACLE IS PLACED IN OPPONENTS FIELD",
		UpgradeRarity::Uncommon,
		false,
		[](Paddle& p, Paddle& opp, Ball& ball)
		{
			opp.PlaceObstacle(50.f, 10.f);
			opp.AddUpgrade(UpgradeType::Obstacles, false);
		} });

	allUpgrades.push_back({
		UpgradeType::Sping,
		"SPING",
		"YOUR HITS APPLY CURVATURE TO THE BALL",
		UpgradeRarity::Uncommon,
		false,
		[](Paddle& p, Paddle& opp, Ball& ball)
		{
			p.AddCurvaturePower(0.5f);
			p.AddUpgrade(UpgradeType::Sping, false);
		} });

	allUpgrades.push_back({
		UpgradeType::Foresight,
		"FORESIGHT",
		"YOU SEE THE BALL TRAJECTORY",
		UpgradeRarity::Uncommon,
		true,
		[](Paddle& p, Paddle& opp, Ball& ball)
		{
			p.EnableForesight();
			p.AddUpgrade(UpgradeType::Foresight, true);
		} });

	// Legendary upgrades:

	allUpgrades.push_back({
		UpgradeType::DumbBuddy,
		"DUMB BUDDY",
		"ADD ONE RECTANGLE WITH DUMMY MOVEMENT",
		UpgradeRarity::Legendary,
		false,
		[](Paddle& p, Paddle& opp, Ball& ball)
		{
			p.AddBuddy();
			p.AddUpgrade(UpgradeType::DumbBuddy, false);
		} });

	allUpgrades.push_back({
		UpgradeType::Crank,
		"CRANK",
		"YOU ARE NEVER EXHAUSTED",
		UpgradeRarity::Legendary,
		true,
		[](Paddle& p, Paddle& opp, Ball& ball)
		{
			p.DisableExhaustion();
			p.AddUpgrade(UpgradeType::Crank, true);
		} });

	allUpgrades.push_back({
		UpgradeType::Extraganza,
		"EXTRAGANZA",
		"YOU CAN NOW MOVE UP AND DOWN",
		UpgradeRarity::Legendary,
		true,
		[](Paddle& p, Paddle& opp, Ball& ball)
		{
			p.EnableUpAndDownMovement();
			p.AddUpgrade(UpgradeType::Extraganza, true);
		} });
}