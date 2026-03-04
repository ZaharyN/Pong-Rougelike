#include "Paddle.h"

Paddle::Paddle(const sf::Vector2f& size, const PaddleScreenPosition screenPos, const sf::Vector2f& startPosition,
	const sf::Color& initialColor, float speed, int windowWidth)
	: initialSpeed(speed), currentSpeed(speed), horizontalDirection(0), screenPosition(screenPos), windowWidth(windowWidth)
{
	body = sf::RectangleShape(size);
	body.setOrigin(body.getGeometricCenter());
	body.setPosition(startPosition);
	body.setFillColor(initialColor);
}

void Paddle::Draw(sf::RenderTarget& target)
{
	target.draw(body);
}

void Paddle::Reset()
{
	currentSpeed = initialSpeed;
	horizontalDirection = 0;
}

void Paddle::SetPosition(const sf::Vector2f& newPosition)
{
	body.setPosition(newPosition);
}

const float Paddle::GetCurrentSpeed() const
{
	return this->currentSpeed;
}

const float Paddle::GetInitialSpeed() const
{
	return this->initialSpeed;
}

const sf::RectangleShape& Paddle::GetBody() const
{
	return this->body;
}
