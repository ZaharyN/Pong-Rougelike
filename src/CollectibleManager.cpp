#include "CollectibleManager.h"

CollectibleManager::CollectibleManager(int windowWidth, int windowHeight)
	: GAMESCREEN_WIDTH(windowWidth), GAMESCREEN_HEIGHT(windowHeight),
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
				player1->CollectEnergy();
				player1->UpdateEnergy(COLLECTIBLE_ENERGY);
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
		collectible->Draw(window);
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
		? COLLECTIBLE_HEIGHT / 2
		: GAMESCREEN_HEIGHT - COLLECTIBLE_HEIGHT / 2;

	float minX;
	float maxX;

	const sf::Vector2f& playerPosition = player->GetBody().getPosition();

	float leftFreeSpace = playerPosition.x - player->GetBody().getSize().x / 2;
	float rightFreeSpace = GAMESCREEN_WIDTH - (playerPosition.x + player->GetBody().getSize().x / 2);

	if (leftFreeSpace >= rightFreeSpace)
	{
		minX = COLLECTIBLE_WIDTH / 2;
		maxX = leftFreeSpace - COLLECTIBLE_WIDTH / 2;
	}
	else
	{
		minX = playerPosition.x + COLLECTIBLE_WIDTH / 2;
		maxX = GAMESCREEN_WIDTH - COLLECTIBLE_WIDTH / 2;
	}

	std::uniform_real_distribution<float> random(minX, maxX);
	x = random(rng);

	float energyRangeModifier = player->GetEnergySpawnRangeModifier();
	float modifiedX = leftFreeSpace >= rightFreeSpace ? x - energyRangeModifier : x + energyRangeModifier;
	float clampedX = std::clamp(modifiedX, COLLECTIBLE_WIDTH / 2, GAMESCREEN_WIDTH - COLLECTIBLE_WIDTH / 2);

	std::unique_ptr<Collectible> collectible = std::make_unique<Collectible>(
		sf::Vector2f{ clampedX,y },
		energyTexture);

	collectibles.push_back(std::move(collectible));
}