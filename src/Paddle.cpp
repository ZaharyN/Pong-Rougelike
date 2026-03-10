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
	currentEnergy = std::clamp(currentEnergy -= energyTake, 0, initialEnergy);

	std::cout << "Player energy:" << currentEnergy << std::endl;
	float ratio = (float)currentEnergy / initialEnergy;

	currentSpeed = initialSpeed * ratio;
	ChangeColorFromRation(ratio);
}

void Paddle::ChangeColorFromRation(float ratio)
{
	float greenValue;
	float redValue;
	
	if (ratio >= 0.75)
	{
		// Magic numbers until I figure out what exactly I want
		redValue = std::clamp(255.f * (1 - ratio) * 4.f, 0.f, 255.f);
		greenValue = 255.f;
	}
	else
	{
		// Magic numbers until I figure out what exactly I want
		greenValue = std::clamp(255 * ratio * 1.33f, 0.f, 255.f);
		redValue = 255.f;
	}

	body.setFillColor(sf::Color(static_cast<uint8_t>(redValue), static_cast<uint8_t>(greenValue), color.b));
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

const PaddleScreenPosition Paddle::GetScreenPosition() const
{
	return this->screenPosition;
}
