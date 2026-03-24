#include "Paddle.h"

Paddle::Paddle(const sf::Vector2f& size, const PaddleScreenPosition screenPos, const sf::Vector2f& startPosition,
	const sf::Color& initialColor, float speed, int windowWidth, int windowHeight, int initialEnergy)
	: initialSpeed(speed), currentSpeed(speed), horizontalDirection(0), screenPosition(screenPos),
	windowWidth(windowWidth), windowHeight(windowHeight), initialEnergy(initialEnergy), currentEnergy(initialEnergy),
	color(initialColor), energyCollected(0), rng(std::random_device{}())
{
	body = sf::RectangleShape(size);
	body.setOrigin(body.getGeometricCenter());
	body.setPosition(startPosition);
	body.setFillColor(initialColor);
}

void Paddle::Draw(sf::RenderTarget& target)
{
	if (isDashing)
	{
		body.setFillColor(sf::Color::Transparent);
		body.setOutlineColor(sf::Color::Cyan);
		body.setOutlineThickness(3.f);
	}
	else
	{
		body.setFillColor(color);
		body.setOutlineThickness(0.f);
	}

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

	color = sf::Color(static_cast<uint8_t>(redValue), static_cast<uint8_t>(greenValue), color.b);
	body.setFillColor(color);
}

void Paddle::CollectEnergy()
{
	energyCollected++;
}

void Paddle::ResetCollectedEnergy()
{
	energyCollected = 0;
}

void Paddle::AddUpgrade(UpgradeType type, bool isUnique)
{
	if (isUnique)
	{
		uniqueUpgrades.insert(type);
		return;
	}

	stackableUpgrades[type]++;
}

bool Paddle::HasUniqueUpgrade(UpgradeType type)
{
	return uniqueUpgrades.find(type) != uniqueUpgrades.end();
}

void Paddle::SetSpeed(float factor)
{
	currentSpeed *= factor;
}

void Paddle::SetSize(float factor)
{
	body.setSize({ body.getSize().x * factor, body.getSize().y });
	body.setOrigin(body.getGeometricCenter());

	float halfWidth = body.getSize().x / 2;
	float clampedX = std::clamp(body.getPosition().x, 0 + halfWidth, windowWidth - halfWidth);

	body.setPosition({ clampedX, body.getPosition().y });
}

void Paddle::SetSpin(float factor)
{
	spinMultiplier *= factor;
}

void Paddle::ModifyEnergySpawnRange(float value)
{
	energyRangeModifier += value;
}

void Paddle::EnableDash()
{
	hasDashUpgrade = true;
}

void Paddle::StartDash()
{
	isDashing = true;
	dashTimer = 0.1f;
	dashSpeedMultiplier = 5.f;
}

void Paddle::PlaceObstacle(float obstacleWidth, float obstacleHeight)
{
	std::uniform_real_distribution<float> random(obstacleWidth / 2.f, windowWidth - obstacleWidth / 2.f);
	float obstacleX = random(rng);
	float obstacleY = windowHeight / 2.f;

	sf::RectangleShape obstacle({ obstacleWidth, obstacleHeight });
	obstacle.setOrigin(obstacle.getGeometricCenter());
	obstacle.setPosition({ obstacleX, obstacleY });
	obstacle.setFillColor(sf::Color(100, 100, 100));
	obstacle.setOutlineThickness(2.f);
	obstacle.setOutlineColor(sf::Color::White);

	obstacles.push_back(obstacle);
}

const float Paddle::GetCurrentSpeed() const
{
	return currentSpeed;
}

const float Paddle::GetInitialSpeed() const
{
	return initialSpeed;
}

int Paddle::GetXDirection() const
{
	return horizontalDirection;
}

const sf::RectangleShape& Paddle::GetBody() const
{
	return body;
}

const int Paddle::GetCollectedEnergy() const
{
	return energyCollected;
}

sf::FloatRect Paddle::GetGlobalBounds() const
{
	return body.getGlobalBounds();
}

const PaddleScreenPosition Paddle::GetScreenPosition() const
{
	return screenPosition;
}

float Paddle::GetSpinMultiplier() const
{
	return spinMultiplier;
}

float Paddle::GetEnergySpawnRangeModifier() const
{
	return energyRangeModifier;
}

const std::vector<sf::RectangleShape>& Paddle::GetObstacles() const
{
	return obstacles;
}

const std::unordered_set<UpgradeType>& Paddle::GetOwnedUniqueUpgrades() const
{
	return uniqueUpgrades;
}
