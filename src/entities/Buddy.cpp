#include "Buddy.h"

Buddy::Buddy(const sf::Vector2f& size, const PaddleScreenPosition screenPos, const sf::Vector2f& startPosition,
	const sf::Color& color, float speed, int windowWidth, int windowHeight, int initialEnergy)
	: Paddle(size, screenPos, startPosition, color, speed, windowWidth, windowHeight, initialEnergy)
{
	horizontalDirection = 1;
}

void Buddy::Update(float deltaT)
{
	float newXPosition = body.getPosition().x + deltaT * horizontalDirection * currentSpeed;

	if (newXPosition >= WINDOW_WIDTH - body.getSize().x / 2.f
		|| newXPosition <= body.getSize().x / 2.f)
	{
		horizontalDirection *= -1;
	}

	newXPosition = std::clamp(newXPosition, body.getSize().x / 2, WINDOW_WIDTH - body.getSize().x / 2);

	SetPosition({ newXPosition, body.getPosition().y });
}