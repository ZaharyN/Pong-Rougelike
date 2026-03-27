#include "Ball.h"

Ball::Ball(float initialRadius, const sf::Vector2f& initialPosition,
	float initialSpeed, int minAngle,
	int maxAngle, const sf::Color& color)
	:INITIAL_RADIUS(initialRadius), INITIAL_MOVEMENT_SPEED(initialSpeed),
	currentMovementSpeed(INITIAL_MOVEMENT_SPEED), currentRadius(initialRadius),
	INITIAL_MIN_ANGLE(minAngle), INITIAL_MAX_ANGLE(maxAngle), color(color),
	rng(std::random_device{}()), colorState(MapColorToState(color))
{
	body = sf::CircleShape(initialRadius);
	body.setOrigin(body.getGeometricCenter());
	body.setPosition(initialPosition);
	body.setFillColor(color);

	ResetAngle();
	SetInitialColorValues(color);
}

void Ball::Update(float deltaT)
{
	horizontalDirection += -currentCurvature * deltaT;

	float length = std::sqrt(horizontalDirection * horizontalDirection + verticalDirection * verticalDirection);
	horizontalDirection /= length;
	verticalDirection /= length;

	if (std::abs(currentCurvature) > 0.01f)
	{
		currentCurvature -= currentCurvature * CURVATURE_DECAY;
	}

	float ballNewX = body.getPosition().x + deltaT * currentMovementSpeed * horizontalDirection;
	float ballNewY = body.getPosition().y + deltaT * currentMovementSpeed * verticalDirection;

	SetPosition({ ballNewX, ballNewY });
	ChangeColor(deltaT);
}

void Ball::Draw(sf::RenderTarget& target)
{
	target.draw(body);
}

void Ball::Reset()
{
	currentMovementSpeed = INITIAL_MOVEMENT_SPEED;
	currentRadius = INITIAL_RADIUS;
	ResetCurvature();
	ResetAngle();
}

void Ball::SwapHorizontalDirection()
{
	horizontalDirection *= -1;
}

void Ball::SwapVerticalDirection()
{
	verticalDirection *= -1;
}

void Ball::IncreaseSpeed()
{
	currentMovementSpeed += currentMovementSpeed * SPEED_MULTIPLIER;
}

void Ball::ApplySpin(float paddleXDirection, float spinMultiplier, float curvaturePower)
{
	horizontalDirection += paddleXDirection * INFLUENCE * spinMultiplier;
	verticalDirection *= -1;

	currentCurvature = paddleXDirection * curvaturePower;

	float length = std::sqrt(horizontalDirection * horizontalDirection + verticalDirection * verticalDirection);

	horizontalDirection /= length;
	verticalDirection /= length;
}

void Ball::SetPosition(const sf::Vector2f& newPosition)
{
	body.setPosition(newPosition);
}

void Ball::SetInitialColorValues(const sf::Color& initialColor)
{
	currR = initialColor.r;
	currG = initialColor.g;
	currB = initialColor.b;
}

void Ball::ChangeColor(float deltaT)
{
	switch (colorState)
	{
	case ColorState::Red:
		currG += COLOR_CHANGE_SPEED * deltaT;
		if (currG >= 255)
		{
			currG = 255;
			colorState = ColorState::Yellow;
		}
		break;

	case ColorState::Yellow:
		currR -= COLOR_CHANGE_SPEED * deltaT;
		if (currR <= 0)
		{
			currR = 0;
			colorState = ColorState::Green;
		}
		break;

	case ColorState::Green:
		currB += COLOR_CHANGE_SPEED * deltaT;
		if (currB >= 255)
		{
			currB = 255;
			colorState = ColorState::Turquoise;
		}
		break;

	case ColorState::Turquoise:
		currG -= COLOR_CHANGE_SPEED * deltaT;
		if (currG <= 0)
		{
			currG = 0;
			colorState = ColorState::Blue;
		}
		break;

	case ColorState::Blue:
		currR += COLOR_CHANGE_SPEED * deltaT;
		if (currR >= 255)
		{
			currR = 255;
			colorState = ColorState::Purple;
		}
		break;

	case ColorState::Purple:
		currB -= COLOR_CHANGE_SPEED * deltaT;
		if (currB <= 0)
		{
			currB = 0;
			colorState = ColorState::Red;
		}
		break;
	}

	body.setFillColor(sf::Color(static_cast<uint8_t>(currR), static_cast<uint8_t>(currG), static_cast<uint8_t>(currB)));
}

void Ball::ResetCurvature()
{
	currentCurvature = 0.f;
}

// Getters:

float Ball::GetCurrentSpeed() const
{
	return currentMovementSpeed;
}

float Ball::GetInitialSpeed() const
{
	return INITIAL_MOVEMENT_SPEED;
}

float Ball::GetCurrentRadius() const
{
	return currentRadius;
}

float Ball::GetVerticalDirection() const
{
	return verticalDirection;
}

float Ball::GetHorizontalDirection() const
{
	return horizontalDirection;
}

const sf::CircleShape& Ball::GetBody() const
{
	return body;
}

sf::FloatRect Ball::GetGlobalBounds() const
{
	return body.getGlobalBounds();
}

sf::FloatRect Ball::GetLocalBounds() const
{
	return body.getLocalBounds();
}

void Ball::ResetAngle()
{
	sf::Angle startingAngle = Ball::GenerateRandomStartingAngle(INITIAL_MIN_ANGLE, INITIAL_MAX_ANGLE);
	horizontalDirection = cos(startingAngle.asRadians());
	verticalDirection = sin(startingAngle.asRadians());
}

sf::Angle Ball::GenerateRandomStartingAngle(int min, int max)
{
	std::uniform_int_distribution random(min, max);

	int randomAngle = random(rng);
	sf::Angle angle = sf::degrees(randomAngle);

	return angle;
}

ColorState Ball::MapColorToState(const sf::Color& initialColor)
{
	if (initialColor.r == 255 && initialColor.g == 255)
		return ColorState::Yellow;

	if (initialColor.g == 255 && initialColor.b == 255)
		return ColorState::Turquoise;

	if (initialColor.b == 255 && initialColor.r == 255)
		return ColorState::Purple;

	if (initialColor.r == 255)
		return ColorState::Red;

	if (initialColor.g == 255)
		return ColorState::Green;

	if (initialColor.b == 255)
		return ColorState::Blue;

	return ColorState::White;
}

float Ball::GetCurvature() const
{
	return currentCurvature;
}