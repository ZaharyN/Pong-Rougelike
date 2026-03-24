#include "CollectibleManager.h"

CollectibleManager::CollectibleManager(int windowWidth, int windowHeight)
	: gameScreenWidth(windowWidth), gameScreenHeight(windowHeight),
	spawnDelay(COLLECTIBLE_SPAWN_DELAY), spawnTimer(0.f),
	rng(std::random_device{}())
{
	if (!energyTexture.loadFromFile("Assets/Textures/energy.png"))
	{
		std::cout << "EnergyTexture could not be loaded" << std::endl;
	}
}

void CollectibleManager::Update(float deltaT, Paddle* player1, Paddle* player2)
{
	spawnTimer += deltaT;

	if (spawnTimer >= spawnDelay)
	{
		std::cout << "Spawning colelctibles" << std::endl;
		SpawnCollectible(player1->GetScreenPosition(), player1);
		SpawnCollectible(player2->GetScreenPosition(), player2);

		spawnTimer = 0.f;
		spawnDelay = COLLECTIBLE_SPAWN_DELAY;
	}
}

void CollectibleManager::CheckCollisions(Paddle* player1, Paddle* player2, AudioManager& audioManager)
{
	std::erase_if(collectibles, [&](const std::unique_ptr<Collectible>& c)
		{
			if (player1->GetGlobalBounds().findIntersection(c->GetBounds()))
			{
				player1->UpdateEnergy(COLLECTIBLE_ENERGY);
				player1->CollectEnergy();
				audioManager.PlaySound("energy_picked");

				return true;
			}
			if (player2->GetGlobalBounds().findIntersection(c->GetBounds()))
			{
				player2->CollectEnergy();
				player2->UpdateEnergy(COLLECTIBLE_ENERGY);
				audioManager.PlaySound("energy_picked");

				return true;
			}
			return false;
		}
	);
}

void CollectibleManager::Draw(sf::RenderTarget& window)
{
	for (const auto& collectible : collectibles)
	{
		if (collectible)
		{
			collectible->Draw(window);
		}
	}
}

void CollectibleManager::Reset()
{
	spawnDelay = COLLECTIBLE_SPAWN_DELAY;
	spawnTimer = 0.0f;

	collectibles.clear();
}

const std::vector<std::unique_ptr<Collectible>>& CollectibleManager::GetCollectibles() const
{
	return collectibles;
}

void CollectibleManager::SpawnCollectible(PaddleScreenPosition screenPos, Paddle* player)
{
	float x;
	float y = screenPos == PaddleScreenPosition::Top
		? 0 + COLLECTIBLE_HEIGHT / 2
		: gameScreenHeight - COLLECTIBLE_HEIGHT / 2;

	float minX;
	float maxX;

	const sf::Vector2f& playerPosition = player->GetBody().getPosition();

	float leftFreeSpace = 0 + playerPosition.x - player->GetBody().getSize().x / 2;
	float rightFreeSpace = gameScreenWidth - (playerPosition.x + player->GetBody().getSize().x / 2);

	if (leftFreeSpace >= rightFreeSpace)
	{
		minX = 0 + COLLECTIBLE_WIDTH / 2;
		maxX = leftFreeSpace - COLLECTIBLE_WIDTH / 2;
	}
	else
	{
		minX = playerPosition.x + COLLECTIBLE_WIDTH / 2;
		maxX = gameScreenWidth - COLLECTIBLE_WIDTH / 2;
	}

	std::uniform_real_distribution<float> random(minX, maxX);
	x = random(rng);

	std::cout << "Energy spawn location X:" << x << std::endl;

	float energyRangeModifier = player->GetEnergySpawnRangeModifier();

	float modifiedX = leftFreeSpace >= rightFreeSpace ? x - energyRangeModifier : x + energyRangeModifier;
	float clampedX = std::clamp(modifiedX, 0 + COLLECTIBLE_WIDTH / 2, gameScreenWidth - COLLECTIBLE_WIDTH / 2);

	std::cout << "Energy spawn location after range modification: " << clampedX << std::endl;

	std::unique_ptr<Collectible> collectible = std::make_unique<Collectible>(
		sf::Vector2f{ clampedX,y },
		energyTexture,
		COLLECTIBLE_WIDTH,
		COLLECTIBLE_HEIGHT,
		sf::Color::Green);

	collectibles.push_back(std::move(collectible));
}