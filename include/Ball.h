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
	Ball(const float& initialRadius, const sf::Vector2f& initialPosition,
		const float& initialSpeed, const int& minAngle,
		const int& maxAngle, const sf::Color& color);

	void Update(const float& deltaT);

	void Draw(sf::RenderTarget& target);

	void Reset();

	void SwapHorizontalDirection();

	void SwapVerticalDireciton();

	void IncreaseSpeed();

	void SetPosition(const sf::Vector2f& newPosition);

	const float& GetCurrentSpeed() const;

	const float& GetCurrentRadius() const;

	const sf::CircleShape& GetBody() const;

	const sf::FloatRect& GetGlobalBounds() const;

	const sf::FloatRect& GetLocalBounds() const;

	~Ball();

private:
	void ResetAngle();
	sf::Angle GenerateRandomStartingAngle(const int& min, const int& max) const;
};