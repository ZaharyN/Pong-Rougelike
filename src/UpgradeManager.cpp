#include "UpgradeManager.h"

UpgradeManager::UpgradeManager(const unsigned int windowWidth, const unsigned int windowHeight)
	: WINDOW_WIDTH(windowWidth), WINDOW_HEIGHT(windowHeight), rng(std::random_device{}())
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
			p.SetSpeed(SONIC_FACTOR);
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
			opp.SetSpeed(SNAIL_FACTOR);
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
			p.SetSize(GOLIATH_FACTOR);
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
			opp.SetSize(MIDGET_FACTOR);
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
			p.SetSpin(MORE_SPIN_FACTOR);
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
			opp.SetSpin(LESS_SPIN_FACTOR);
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
			p.ModifyEnergySpawnRange(NEAR_FACTOR);
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
			opp.ModifyEnergySpawnRange(FAR_FACTOR);
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
			sf::Color placerColor = opp.GetScreenPosition() == PaddleScreenPosition::Top
				? sf::Color::Green
				: sf::Color::Red;
			opp.PlaceObstacle(OBSTACLE_WIDTH, OBSTACLE_HEIGHT, placerColor);
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
			p.AddCurvaturePower(CURVATURE_POWER);
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
		[&](Paddle& p, Paddle& opp, Ball& ball)
		{
			auto buddy = std::make_unique<Buddy>(
			sf::Vector2f(BUDDY_WIDTH, BUDDY_HEIGHT),
			p.GetScreenPosition(),
			sf::Vector2f(p.GetBody().getPosition()),
			sf::Color(255, 255, 255, 150),
			p.GetInitialSpeed() / 2.f,
			WINDOW_WIDTH, WINDOW_HEIGHT, 100);

			p.AddBuddy(std::move(buddy));
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