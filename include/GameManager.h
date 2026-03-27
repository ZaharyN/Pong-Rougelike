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
	static constexpr unsigned int WINDOW_WIDTH = 1200;
	static constexpr unsigned int WINDOW_HEIGHT = 600;
	static constexpr unsigned int COLLECTIBLE_COUNT_FOR_UPGRADE = 3;

	static constexpr float BALL_RADIUS = 5.f;
	static constexpr float PLAYER_WIDTH = 100.f;
	static constexpr float PLAYER_HEIGHT = 10.f;

	static constexpr unsigned int INITIAL_MIN_ANGLE = 45;
	static constexpr unsigned int INITIAL_MAX_ANGLE = 135;

	static constexpr float PLAYER_INITIAL_SPEED = 300.f;
	static constexpr float BALL_INITIAL_SPEED = 300.f;

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

	void CheckWallCollisions();
	void CheckPaddleCollisions();
	void CheckBuddyCollisions();
	void CheckObstacleCollisions();
	void CheckDeadZone();
	void CheckCollectibleCollisions();
public:
	GameManager();
	void Run();
	~GameManager() = default;
};