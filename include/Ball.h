#pragma once
#include <SFML/Graphics.hpp>
#include "Types.h"
#include <iostream>
#include <random>
#include <cmath>

class Ball
{
	static constexpr float INFLUENCE = 0.3f;
	static constexpr float SPEED_MULTIPLIER = 0.01f;
	static constexpr int COLOR_CHANGE_SPEED = 100;
	static constexpr float CURVATURE_DECAY = 0.05F;

	sf::CircleShape body;
	sf::Color color;
	std::mt19937 rng;

	ColorState colorState;
	float currR, currG, currB;

	const float INITIAL_MOVEMENT_SPEED;
	float currentMovementSpeed;

	const int INITIAL_MIN_ANGLE;
	const int INITIAL_MAX_ANGLE;

	const float INITIAL_RADIUS;
	float currentRadius;

	float horizontalDirection;
	float verticalDirection;
	float currentCurvature = 0.f;

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

	void ApplySpin(float paddleXDirection, float spinMultiplier, float curvaturePower);

	void SetPosition(const sf::Vector2f& newPosition);

	void SetInitialColorValues(const sf::Color& initialColor);

	void ChangeColor(float deltaT);

	void ResetCurvature();

	float GetCurrentSpeed() const;

	float GetInitialSpeed() const;

	float GetCurrentRadius() const;

	float GetVerticalDirection() const;

	float GetHorizontalDirection() const;

	const sf::CircleShape& GetBody() const;

	sf::FloatRect GetGlobalBounds() const;

	sf::FloatRect GetLocalBounds() const;

	float GetCurvature() const;

	~Ball() = default;

private:
	void ResetAngle();

	sf::Angle GenerateRandomStartingAngle(int min, int max);

	static ColorState MapColorToState(const sf::Color& initialColor);
};