#pragma once
#include <SFML/Graphics.hpp>
#include "Types.h"
#include "Ball.h"
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <list>
#include <random>
#include <memory>

class Paddle
{
protected:
	static constexpr float DASH_COOLDOWN_DURATION = 5.f;
	static constexpr float DASH_DURATION = 0.1f;

	static constexpr float BUDDY_WIDTH = 80.f;
	static constexpr float BUDDY_HEIGHT = 10.f;

	static constexpr float FORESIGHT_DOT_SPACING = 30.f;
	static constexpr float FORESIGHT_DOT_RADIUS = 3.f;

	const PaddleScreenPosition SCREEN_POSITION;
	const unsigned int WINDOW_WIDTH;
	const unsigned int WINDOW_HEIGHT;
	const float INITIAL_SPEED;
	const int INITIAL_ENERGY;

	sf::RectangleShape body;
	sf::Color color;
	float currentSpeed;
	int currentEnergy;
	int horizontalDirection;
	int verticalDirection;
	int energyCollected;

	// Modifiables:
	std::mt19937 rng;
	std::unordered_set<UpgradeType> uniqueUpgrades;
	std::unordered_map<UpgradeType, int> stackableUpgrades;
	std::vector<sf::RectangleShape> obstacles;
	std::vector<std::unique_ptr<Paddle>> buddies;
	std::list<sf::CircleShape> foresightDots;
	float reducedCollectibleSpawnRange = 0.f;
	float force = 1.0f;
	float spinMultiplier = 1.0f;
	float energyRangeModifier = 0.f;
	bool hasDashUpgrade = false;
	bool isDashing = false;
	float dashTimer = DASH_DURATION;
	float dashCooldown = 0.f; 
	float dashSpeedMultiplier = 1.f;
	bool isNeverExhausted = false;
	bool canMoveUpAndDown = false;
	bool hasForesight = false;
	float curvaturePower = 0.f;

public:
	Paddle(const sf::Vector2f& size, const PaddleScreenPosition screenPos, const sf::Vector2f& startPosition, 
		const sf::Color& initialColor, float speed, int windowWidth, int windowHeight, int initialEnergy);

	virtual void Update(float deltaT) = 0;

	virtual void Draw(sf::RenderTarget& target);

	virtual void Reset();

	void SetPosition(const sf::Vector2f& newPosition);

	void UpdateEnergy(int energyTaken);

	void CollectEnergy();

	virtual ~Paddle() = default;

	float GetCurrentSpeed() const;

	float GetInitialSpeed() const;

	int GetXDirection() const;

	const sf::RectangleShape& GetBody() const;

	PaddleScreenPosition GetScreenPosition() const;

	int GetCollectedEnergy() const;

	void ResetCollectedEnergy();

	sf::FloatRect GetGlobalBounds() const;

	float GetSpinMultiplier() const;

	float GetEnergySpawnRangeModifier() const;

	const std::vector<sf::RectangleShape>& GetObstacles()  const;

	const std::unordered_set<UpgradeType>& GetOwnedUniqueUpgrades() const;

	const std::vector<std::unique_ptr<Paddle>>& GetBuddies() const;

	bool HasForesight() const;

	float GetCurvaturePower() const;

	// Modifying methods:
	void AddUpgrade(UpgradeType type, bool isUnique);

	bool HasUniqueUpgrade(UpgradeType type) const;

	void SetSpeed(float factor);

	void SetSize(float factor);

	void SetSpin(float factor);

	void ModifyEnergySpawnRange(float value);

	void EnableDash();

	void StartDash();

	void PlaceObstacle(float obstacleWidth, float obstacleHeight, const sf::Color outlineColor);

	void DisableExhaustion();

	void EnableUpAndDownMovement();

	void AddBuddy();

	void EnableForesight();

	void ComputeForesight(const Ball& ballRef);

	void DrawForesight(sf::RenderTarget& target) const;

	void TrimForesight(float ballY, float verticalDirection);

	void AddCurvaturePower(float power);
};
