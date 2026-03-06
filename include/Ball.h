#pragma once
#include <SFML/Graphics.hpp>
#include "Types.h"
#include <iostream>
#include <random>
#include <cmath>

class Ball
{
	const float INFLUENCE = 0.3f;
	const float SPEED_MULTIPLIER = 0.01f;
	const int COLOR_CHANGE_SPEED = 100;

	sf::CircleShape body;
	sf::Color color;
	std::mt19937 rng;

	ColorState colorState;
	float currR, currG, currB;

	const float initialMovementSpeed;
	float currentMovementSpeed;

	const int initialMinAngle;
	const int initialMaxAngle;

	const float initialRadius;
	float currentRadius;

	float horizontalDirection;
	float verticalDirection;

public:
	Ball(float initialRadius, const sf::Vector2f& initialPosition,
		float initialSpeed, int minAngle,
		int maxAngle, const sf::Color& color);

	void Update(float deltaT);

	void Draw(sf::RenderTarget& target);

	void Reset();

	void SwapHorizontalDirection();

	void SwapVerticalDirection();

	void IncreaseSpeed();

	void ApplySpin(float paddleXDirection);

	void SetPosition(const sf::Vector2f& newPosition);

	void SetInitialColorValues(const sf::Color& initialColor);

	void ChangeColor(float deltaT);

	float GetCurrentSpeed() const;

	float GetInitialSpeed() const;

	float GetCurrentRadius() const;

	const sf::CircleShape& GetBody() const;

	sf::FloatRect GetGlobalBounds() const;

	sf::FloatRect GetLocalBounds() const;

	~Ball() = default;

private:
	void ResetAngle();

	sf::Angle GenerateRandomStartingAngle(int min, int max);

	static ColorState MapColorToState(const sf::Color& initialColor);
};