#include "Player.h"
#include <iostream>

void Player::Update(const float& deltaT)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left)
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A))
	{
		horizontalDirection = -1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right)
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D))
	{
		horizontalDirection = 1;
	}

	float newPosition = body.getPosition().x + deltaT * currentSpeed * horizontalDirection;
	this->SetPosition({ newPosition, body.getPosition().y });
}