#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

class Ball
{
	sf::CircleShape body;
	sf::Color color;

	const float speedMultiplier = 0.01f;
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

	void SwapVerticalDireciton();

	void IncreaseSpeed();

	void SetPosition(const sf::Vector2f& newPosition);

	float GetCurrentSpeed() const;

	float GetCurrentRadius() const;

	const sf::CircleShape& GetBody() const;

	const sf::FloatRect& GetGlobalBounds() const;

	const sf::FloatRect& GetLocalBounds() const;

	~Ball() = default;

private:
	void ResetAngle();

	sf::Angle GenerateRandomStartingAngle(int min, int max) const;
};