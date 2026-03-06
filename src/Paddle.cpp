#include "Paddle.h"

Paddle::Paddle(const sf::Vector2f& size, const PaddleScreenPosition screenPos, const sf::Vector2f& startPosition,
	const sf::Color& initialColor, float speed, int windowWidth, int initialEnergy)
	: initialSpeed(speed), currentSpeed(speed), horizontalDirection(0), screenPosition(screenPos), 
	  windowWidth(windowWidth), initialEnergy(initialEnergy), currentEnergy(initialEnergy), color(initialColor)
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

void Paddle::ReduceEnergy(int energyTake)
{
	currentEnergy -= energyTake;

	float ratio = (float)currentEnergy / initialEnergy;

	currentSpeed *= ratio;
	ChangeColorFromRation(ratio);
}

void Paddle::ChangeColorFromRation(float ratio)
{
	float greenValue;
	float redValue;
	if (ratio >= 0.5)
	{
		// Increase the red
		redValue = std::clamp(255.f * (1 - ratio) * 2.f, 0.f, 255.f);
		greenValue = color.g;
	}
	else
	{
		// Decrease the green
		greenValue = std::clamp(255 * ratio * 2, 0.f, 255.f);
		redValue = color.r;
	}

	body.setFillColor(sf::Color(static_cast<uint8_t>(greenValue), static_cast<uint8_t>(redValue), color.b));
}

const float Paddle::GetCurrentSpeed() const
{
	return this->currentSpeed;
}

const float Paddle::GetInitialSpeed() const
{
	return this->initialSpeed;
}

int Paddle::GetXDirection() const
{
	return this->horizontalDirection;
}

const sf::RectangleShape& Paddle::GetBody() const
{
	return this->body;
}

sf::FloatRect Paddle::GetGlobalBounds() const
{
	return this->body.getGlobalBounds();
}
