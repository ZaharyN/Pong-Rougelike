#include "Enemy.h"

Enemy::Enemy(const sf::Vector2f& size, const PaddleScreenPosition screenPos, const sf::Vector2f& startPosition,
	const sf::Color& initialColor, float speed, const Ball& ballRef,int windowWidth, int initialEnergy)
	: Paddle(size, screenPos, startPosition, initialColor, speed, windowWidth, initialEnergy), ball(ballRef)
{
	horizontalDirection = 1;
}

void Enemy::Update(float deltaT)
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

	newPosition = std::clamp(newPosition, 0 + body.getSize().x / 2, windowWidth - body.getSize().x / 2);
	this->SetPosition({ newPosition, body.getPosition().y });
}