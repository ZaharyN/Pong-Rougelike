#pragma once
#include "SFML/Graphics.hpp"
#include "Collectible.h"
#include "Types.h"
#include "AudioManager.h"
#include "Paddle.h"
#include <vector>
#include <memory>
#include <random>

class CollectibleManager
{
	const float COLLECTIBLE_WIDTH = 40.f;
	const float COLLECTIBLE_HEIGHT = 40.f;
	const float COLLECTIBLE_SPAWN_DELAY = 10.f;
	const float COLLECTIBLE_ENERGY = 5.f;

	std::mt19937 rng;
	float spawnDelay;
	float spawnTimer;

	int gameScreenWidth;
	int gameScreenHeight;

	sf::Texture energyTexture;

	std::vector<std::unique_ptr<Collectible>> collectibles;

public:
	CollectibleManager(int windowWidth, int windowHeight);
	void Update(float deltaT, Paddle* player1, Paddle* player2);
	void CheckCollisions(Paddle* player1, Paddle* player2, AudioManager& audioManager);
	void Draw(sf::RenderTarget& window);
	const std::vector<std::unique_ptr<Collectible>>& GetCollectibles() const;

private:
	void SpawnCollectible(PaddleScreenPosition screenPos, Paddle* player);
};