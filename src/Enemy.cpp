#include "Enemy.h"

Enemy::Enemy(const sf::Vector2f& size, const PaddleScreenPosition screenPos, const sf::Vector2f& startPosition,
	const sf::Color& initialColor, const float& speed, const Ball& ballRef)
	: Paddle(size, screenPos, startPosition, initialColor, speed), ball(ballRef)
{
	horizontalDirection = 1;
}

void Enemy::Update(const float& deltaT)
{
	float currentStep = currentSpeed * deltaT;
	float distance = ball.GetBody().getPosition().x - body.getPosition().x;
	
	float newPosition;
	if (currentStep > abs(distance))
	{
		newPosition = ball.GetBody().getPosition().x;
	}
	else
	{
		horizontalDirection = distance > 1 ? 1 : -1;
		newPosition = body.getPosition().x + currentStep * horizontalDirection;
	}
	
	this->SetPosition({newPosition, body.getPosition().y});
}