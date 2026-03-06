#pragma once
#include "Paddle.h"

class Player : public Paddle
{
	PlayerControls controls;
public:
	Player(const sf::Vector2f& size, const PaddleScreenPosition screenPos, const sf::Vector2f& startPosition,
		const sf::Color& initialColor, float speed, int windowWidth, int initialEnergy, const PlayerControls& playerControls);

	void Update(float deltaT) override;
};