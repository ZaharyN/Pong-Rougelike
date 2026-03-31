#include "Player.h"
#include <iostream>

Player::Player(const sf::Vector2f& size, const PaddleScreenPosition screenPos, const sf::Vector2f& startPosition,
	const sf::Color& initialColor, float speed, int windowWidth, int windowHeight, int initialEnergy, const PlayerControls& playerControls)
	: Paddle(size, screenPos, startPosition, initialColor, speed, windowWidth, windowHeight, initialEnergy), controls(playerControls)
{

}

void Player::Update(float deltaT)
{
	horizontalDirection = 0;
	verticalDirection = 0;

	if (stats.dashCooldown > 0) stats.dashCooldown -= deltaT;

	if (stats.isDashing)
	{
		stats.dashTimer -= deltaT;
		if (stats.dashTimer <= 0)
		{
			stats.isDashing = false;
			stats.dashSpeedMultiplier = 1.f;
			stats.dashCooldown = 5.f;
		}
	}

	if (sf::Keyboard::isKeyPressed(controls.leftPrimary) || sf::Keyboard::isKeyPressed(controls.leftSecondary))
		horizontalDirection = -1;

	if (sf::Keyboard::isKeyPressed(controls.rightPrimary) || sf::Keyboard::isKeyPressed(controls.rightSecondary))
		horizontalDirection = 1;

	if (stats.canMoveUpAndDown)
	{
		if (sf::Keyboard::isKeyPressed(controls.upPrimary) || sf::Keyboard::isKeyPressed(controls.upSecondary))
			verticalDirection = -1;

		if (sf::Keyboard::isKeyPressed(controls.downPrimary) || sf::Keyboard::isKeyPressed(controls.downSecondary))
			verticalDirection = 1;
	}

	if (sf::Keyboard::isKeyPressed(controls.dashPrimary) || sf::Keyboard::isKeyPressed(controls.dashSecondary))
	{
		if (stats.hasDashUpgrade && stats.dashCooldown <= 0 && !stats.isDashing)
		{
			StartDash();
		}
	}

	float speed = currentSpeed * stats.dashSpeedMultiplier;
	float newXPosition = body.getPosition().x + deltaT * horizontalDirection * speed;
	float newYPosition = body.getPosition().y + deltaT * verticalDirection * speed;

	float minY;
	float maxY;

	if (SCREEN_POSITION == PaddleScreenPosition::Top)
	{
		minY = body.getSize().y / 2.f;
		maxY = WINDOW_HEIGHT / 2.f - body.getSize().y / 2.f;
	}
	else
	{
		minY = WINDOW_HEIGHT / 2.f + body.getSize().y / 2.f;
		maxY = WINDOW_HEIGHT - body.getSize().y / 2.f;
	}

	newYPosition = std::clamp(newYPosition, minY, maxY);
	newXPosition = std::clamp(newXPosition, body.getSize().x / 2, WINDOW_WIDTH - body.getSize().x / 2);

	this->SetPosition({ newXPosition, newYPosition });
}