#include "Ball.h"

Ball::Ball(const float& initialRadius, const sf::Vector2f& initialPosition,
	const float& initialSpeed, const int& minAngle,
	const int& maxAngle, const sf::Color& color)
	:initialRadius(initialRadius), initialMovementSpeed(initialSpeed),
	currentMovementSpeed(initialMovementSpeed), currentRadius(initialRadius),
	initialMinAngle(minAngle), initialMaxAngle(maxAngle), color(color)
{
	body = sf::CircleShape(initialRadius);
	body.setOrigin(body.getGeometricCenter());
	body.setPosition(initialPosition);
	body.setFillColor(color);

	ResetAngle();
}

void Ball::Update(const float& deltaT)
{
	float ballX = body.getPosition().x;
	float ballY = body.getPosition().y;
	float ballNewX = ballX + deltaT * currentMovementSpeed * horizontalDirection;
	float ballNewY = ballY + deltaT * currentMovementSpeed * verticalDirection;

	SetPosition({ ballNewX, ballNewY });
}

void Ball::Draw(sf::RenderTarget& target)
{
	target.draw(body);
}

void Ball::Reset()
{
	currentMovementSpeed = initialMovementSpeed;
	currentRadius = initialRadius;
	ResetAngle();
}

void Ball::SwapHorizontalDirection()
{
	horizontalDirection *= -1;
}

void Ball::SwapVerticalDireciton()
{
	verticalDirection *= -1;
}

void Ball::IncreaseSpeed()
{
	currentMovementSpeed += currentMovementSpeed * speedMultiplier;
}

void Ball::SetPosition(const sf::Vector2f& newPosition)
{
	body.setPosition(newPosition);
}

const float& Ball::GetCurrentSpeed() const
{
	return this->currentMovementSpeed;
}

const float& Ball::GetCurrentRadius() const
{
	return this->currentRadius;
}

const sf::CircleShape& Ball::GetBody() const
{
	return this->body;
}

const sf::FloatRect& Ball::GetGlobalBounds() const
{
	return this->body.getGlobalBounds();
}

const sf::FloatRect& Ball::GetLocalBounds() const
{
	return this->body.getLocalBounds();
}

void Ball::ResetAngle()
{
	sf::Angle startingAngle = Ball::GenerateRandomStartingAngle(initialMinAngle, initialMaxAngle);
	horizontalDirection = cos(startingAngle.asRadians());
	verticalDirection = sin(startingAngle.asRadians());
}

sf::Angle Ball::GenerateRandomStartingAngle(const int& min, const int& max) const
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution random(min, max);

	int randomAngle = random(mt);
	sf::Angle angle = sf::degrees(randomAngle);

	return angle;
}