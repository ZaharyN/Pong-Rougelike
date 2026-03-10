#include "CollectibleManager.h"

CollectibleManager::CollectibleManager(int windowWidth, int windowHeight)
	: gameScreenWidth(windowWidth), gameScreenHeight(windowHeight),
	spawnDelay(COLLECTIBLE_SPAWN_DELAY), spawnTimer(0.f),
	rng(std::random_device{}())
{
	std::cout << "CollectibleManagaer initialized " << std::endl;
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

void CollectibleManager::CheckCollisions(Paddle* player1, Paddle* player2)
{
	std::erase_if(collectibles, [&](const std::unique_ptr<Collectible>& c)
		{
			if (player1->GetGlobalBounds().findIntersection(c->GetBounds()))
			{
				player1->ReduceEnergy(-COLLECTIBLE_ENERGY);
				std::cout << "Player1 collectible removed " << std::endl;
				return true;
			}
			if (player2->GetGlobalBounds().findIntersection(c->GetBounds()))
			{
				player2->ReduceEnergy(-COLLECTIBLE_ENERGY);
				std::cout << "Player2 collectible removed " << std::endl;
				return true;
			}
			return false;
		});
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

	std::unique_ptr<Collectible> collectible = std::make_unique<Collectible>(
		sf::Vector2f{ x,y },
		COLLECTIBLE_WIDTH,
		COLLECTIBLE_HEIGHT,
		sf::Color::Green);

	collectibles.push_back(std::move(collectible));
}

//const sf::Vector2f CollectibleManager::GenerateRandomPos()
//{
//
//}