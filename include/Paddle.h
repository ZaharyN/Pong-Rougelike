#pragma once
#include <SFML/Graphics.hpp>
#include "Types.h"
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <random>

class Paddle
{
protected:
	sf::RectangleShape body;
	sf::Color color;
	PaddleScreenPosition screenPosition;
	const float initialSpeed;
	float currentSpeed;
	int initialEnergy;
	int currentEnergy;
	int horizontalDirection;
	int windowWidth;
	int windowHeight;
	int energyCollected;

	// Modifiables:
	std::mt19937 rng;
	std::unordered_set<UpgradeType> uniqueUpgrades;
	std::unordered_map<UpgradeType, int> stackableUpgrades;
	std::vector<sf::RectangleShape> obstacles;
	float reducedCollectibleSpawnRange = 0.f;
	float force = 1.0f;
	float spinMultiplier = 1.0f;
	float energyRangeModifier = 0.f;
	bool hasDashUpgrade = false;
	bool isDashing = false;
	float dashTimer = 0.1f;
	float dashCooldown = 0.f; 
	float dashSpeedMultiplier = 1.f;
	bool isNeverExhausted = false;

public:
	Paddle(const sf::Vector2f& size, const PaddleScreenPosition screenPos, const sf::Vector2f& startPosition, 
		const sf::Color& initialColor, float speed, int windowWidth, int windowHeight, int initialEnergy);

	virtual void Update(float deltaT) = 0;

	virtual void Draw(sf::RenderTarget& target);

	virtual void Reset();

	void SetPosition(const sf::Vector2f& newPosition);

	void UpdateEnergy(int energyTaken);

	void CollectEnergy();

	void ChangeColorFromRation(float ratio);

	virtual ~Paddle() = default;

	const float GetCurrentSpeed() const;

	const float GetInitialSpeed() const;

	int GetXDirection() const;

	const sf::RectangleShape& GetBody() const;

	const PaddleScreenPosition GetScreenPosition() const;

	const int GetCollectedEnergy() const;

	void ResetCollectedEnergy();

	sf::FloatRect GetGlobalBounds() const;

	float GetSpinMultiplier() const;

	float GetEnergySpawnRangeModifier() const;

	const std::vector<sf::RectangleShape>& GetObstacles()  const;

	const std::unordered_set<UpgradeType>& GetOwnedUniqueUpgrades() const;

	// Modifying methods:
	void AddUpgrade(UpgradeType type, bool isUnique);

	bool HasUniqueUpgrade(UpgradeType type);

	void SetSpeed(float factor);

	void SetSize(float factor);

	void SetSpin(float factor);

	void ModifyEnergySpawnRange(float value);

	void EnableDash();

	void StartDash();

	void PlaceObstacle(float obstacleWidth, float obstacleHeight);

	void DisableExhaustion();
};
