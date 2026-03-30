#pragma once
#include "Paddle.h"
#include "Ball.h"

class Enemy : public Paddle
{
	static constexpr float MIN_DASH_THRESHOLD = 150.f;

	const Ball& ball;
public:
	Enemy(const sf::Vector2f& size, const PaddleScreenPosition screenPos, const sf::Vector2f& startPosition, 
		const sf::Color& initialColor, float speed, const Ball& ballRef, int windowWidth, int windowHeight, int initialEnergy);

	void Update(float deltaT) override;
};