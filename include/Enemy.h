#pragma once
#include "Paddle.h"
#include "Ball.h"

class Enemy : public Paddle
{
	const Ball& ball;
public:
	Enemy(const sf::Vector2f& size, const PaddleScreenPosition screenPos, const sf::Vector2f& startPosition, 
		const sf::Color& initialColor, const float& speed, const Ball& ballRef, int windowWidth);

	void Update(float deltaT) override;
};