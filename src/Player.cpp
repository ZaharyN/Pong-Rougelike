#include "Player.h"
#include <iostream>

Player::Player(const sf::Vector2f& size, const PaddleScreenPosition screenPos, const sf::Vector2f& startPosition,
	const sf::Color& initialColor, float speed, int windowWidth, int initialEnergy, const PlayerControls& playerControls)
	: Paddle(size, screenPos, startPosition, initialColor, speed, windowWidth, initialEnergy), controls(playerControls)
{

}

void Player::Update(float deltaT)
{
	horizontalDirection = 0;

	if (dashCooldown > 0) dashCooldown -= deltaT;

	if (isDashing)
	{
		dashTimer -= deltaT;
		if (dashTimer <= 0)
		{
			isDashing = false;
			dashSpeedMultiplier = 1.f;
			dashCooldown = 5.f;
		}
	}

	if (sf::Keyboard::isKeyPressed(controls.leftPrimary) || sf::Keyboard::isKeyPressed(controls.leftSecondary))
		horizontalDirection = -1;

	if (sf::Keyboard::isKeyPressed(controls.rightPrimary) || sf::Keyboard::isKeyPressed(controls.rightSecondary))
		horizontalDirection = 1;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space))
	{
		if (hasDashUpgrade && dashCooldown <= 0 && !isDashing)
		{
			StartDash();
		}
	}

	float speed = currentSpeed * dashSpeedMultiplier;
	float newPosition = body.getPosition().x + deltaT * horizontalDirection * speed;

	newPosition = std::clamp(newPosition, 0 + body.getSize().x / 2, windowWidth - body.getSize().x / 2);
	this->SetPosition({ newPosition, body.getPosition().y });
}