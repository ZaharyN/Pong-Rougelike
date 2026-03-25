#pragma once
#include "Paddle.h"

class Buddy : public Paddle
{
public:
	Buddy(const sf::Vector2f& size, const PaddleScreenPosition screenPos, const sf::Vector2f& startPosition,
		const sf::Color& color, float speed, int windowWidth, int windowHeight, int initialEnergy);

	void Update(float deltaT) override;
};