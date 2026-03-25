#include "Buddy.h"

Buddy::Buddy(const sf::Vector2f& size, const PaddleScreenPosition screenPos, const sf::Vector2f& startPosition,
	const sf::Color& color, float speed, int windowWidth, int windowHeight, int initialEnergy)
	: Paddle(size, screenPos, startPosition, color, speed, windowWidth, windowHeight, initialEnergy)
{
	body = sf::RectangleShape(size);
	body.setOrigin(body.getGeometricCenter());
	body.setPosition(startPosition);
	body.setFillColor(color);

	horizontalDirection = 1;
}

void Buddy::Update(float deltaT)
{
	float newXPosition = body.getPosition().x + deltaT * horizontalDirection * currentSpeed;

	if (newXPosition >= windowWidth - body.getSize().x / 2.f
		|| newXPosition <= 0 + body.getSize().x / 2.f)
	{
		horizontalDirection *= -1;
	}

	newXPosition = std::clamp(newXPosition, 0 + body.getSize().x / 2, windowWidth - body.getSize().x / 2);

	SetPosition({ newXPosition, body.getPosition().y });
}