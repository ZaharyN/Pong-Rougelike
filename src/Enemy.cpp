#include "Enemy.h"

Enemy::Enemy(const sf::Vector2f& size, const PaddleScreenPosition screenPos, const sf::Vector2f& startPosition,
	const sf::Color& initialColor, float speed, const Ball& ballRef,int windowWidth, int windowHeight, int initialEnergy)
	: Paddle(size, screenPos, startPosition, initialColor, speed, windowWidth, windowHeight, initialEnergy), ball(ballRef)
{
	horizontalDirection = 1;
}

void Enemy::Update(float deltaT)
{
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

	float distance = ball.GetBody().getPosition().x - body.getPosition().x;

	if (hasDashUpgrade && !isDashing && dashCooldown <= 0)
	{
		if (std::abs(distance) > 150.f)
		{
			StartDash();
		}
	}

	float speed = currentSpeed * dashSpeedMultiplier;
	float currentStep = speed * deltaT;
	float newPosition;

	if (currentStep > abs(distance))
	{
		newPosition = ball.GetBody().getPosition().x;
		horizontalDirection = 0;
	}
	else
	{
		horizontalDirection = distance > 1 ? 1 : -1;
		newPosition = body.getPosition().x + currentStep * horizontalDirection;
	}

	newPosition = std::clamp(newPosition, 0 + body.getSize().x / 2.f, windowWidth - body.getSize().x / 2.f);
	SetPosition({ newPosition, body.getPosition().y });
}