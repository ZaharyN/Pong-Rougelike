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
	static constexpr float COLLECTIBLE_WIDTH = 40.f;
	static constexpr float COLLECTIBLE_HEIGHT = 40.f;
	static constexpr float COLLECTIBLE_SPAWN_DELAY = 10.f;
	static constexpr int COLLECTIBLE_ENERGY = 5.f;

	const unsigned int GAMESCREEN_WIDTH;
	const unsigned int GAMESCREEN_HEIGHT;

	std::mt19937 rng;
	float spawnDelay;
	float spawnTimer;

	sf::Texture energyTexture;

	std::vector<std::unique_ptr<Collectible>> collectibles;

public:
	CollectibleManager(int windowWidth, int windowHeight);
	void Update(float deltaT, Paddle* player1, Paddle* player2);
	void CheckCollisions(Paddle* player1, Paddle* player2, AudioManager& audioManager);
	void Draw(sf::RenderTarget& window);
	const std::vector<std::unique_ptr<Collectible>>& GetCollectibles() const;
	void Reset();

private:
	void SpawnCollectible(PaddleScreenPosition screenPos, Paddle* player);
};