#pragma once
#include "Paddle.h"
#include "Ball.h"
#include "AudioManager.h"
#include "UIManager.h"
#include "CollectibleManager.h"
#include "UpgradeManager.h"
#include "Types.h"
#include "Player.h"
#include "Enemy.h"
#include <memory>
#include <vector>

class GameManager
{
	const unsigned int windowWidth = 1200;
	const unsigned int windowHeight = 600;
	const unsigned int collectibleCountForUpgrade = 1;

	const float ballRadius = 5.f;
	const float playerLength = 100.f;
	const float playerHeight = 10.f;

	const int initialMinAngle = 45;
	const int initialMaxAngle = 135;

	const float playerSpeed= 300.f;
	const float ballInitialSpeed = 300.f;

	sf::RenderWindow gameWindow;
	sf::Clock clock;
	GameState gameState = GameState::Menu;

	std::unique_ptr<AudioManager> audioManager;
	std::unique_ptr<UIManager> uiManager;
	std::unique_ptr<CollectibleManager> collectibleManager;
	std::unique_ptr<UpgradeManager> upgradeManager;

	std::unique_ptr<Ball> ball;
	std::unique_ptr<Paddle> player1;
	std::unique_ptr<Paddle> player2;

	Paddle* upgradeRecipient = nullptr;
	Paddle* upgradeVictim = nullptr;
	std::vector<Upgrade> currentUpgradeOptions;

	void ProcessEvents();
	void Update(float deltaT);
	void CheckCollisions();
	void Render();
	void StartGame(GameMode gameMode);

public:
	GameManager();
	void Run();
	~GameManager() = default;
};