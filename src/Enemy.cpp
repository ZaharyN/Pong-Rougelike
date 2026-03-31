#include "Enemy.h"

Enemy::Enemy(const sf::Vector2f& size, const PaddleScreenPosition screenPos, const sf::Vector2f& startPosition,
	const sf::Color& initialColor, float speed, const Ball& ballRef,int windowWidth, int windowHeight, int initialEnergy)
	: Paddle(size, screenPos, startPosition, initialColor, speed, windowWidth, windowHeight, initialEnergy), ball(ballRef)
{
	horizontalDirection = 1;
}

void Enemy::Update(float deltaT)
{
	if (stats.dashCooldown > 0) stats.dashCooldown -= deltaT;

	if (stats.isDashing)
	{
		stats.dashTimer -= deltaT;
		if (stats.dashTimer <= 0)
		{
			stats.isDashing = false;
			stats.dashSpeedMultiplier = 1.f;
			stats.dashCooldown = DASH_COOLDOWN_DURATION;
		}
	}

	float distance = ball.GetBody().getPosition().x - body.getPosition().x;

	if (stats.hasDashUpgrade && !stats.isDashing && stats.dashCooldown <= 0)
	{
		if (std::abs(distance) > MIN_DASH_THRESHOLD)
		{
			StartDash();
		}
	}

	float speed = currentSpeed * stats.dashSpeedMultiplier;
	float currentStep = speed * deltaT;
	float newPosition;

	if (currentStep > abs(distance))
	{
		newPosition = ball.GetBody().getPosition().x;
		horizontalDirection = 0;
	}
	else
	{
		horizontalDirection = distance > 0 ? 1 : -1;
		newPosition = body.getPosition().x + currentStep * horizontalDirection;
	}

	newPosition = std::clamp(newPosition, body.getSize().x / 2.f, WINDOW_WIDTH - body.getSize().x / 2.f);
	SetPosition({ newPosition, body.getPosition().y });
}