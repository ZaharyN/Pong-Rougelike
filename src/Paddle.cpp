#include "Paddle.h"
#include "Buddy.h"

Paddle::Paddle(const sf::Vector2f& size, const PaddleScreenPosition screenPos, const sf::Vector2f& startPosition,
	const sf::Color& initialColor, float speed, int windowWidth, int windowHeight, int initialEnergy)
	: initialSpeed(speed), currentSpeed(speed), horizontalDirection(0), verticalDirection(0), screenPosition(screenPos),
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
	ResetCollectedEnergy();

	// Reset modifiers:
	uniqueUpgrades.clear();
	stackableUpgrades.clear();
	obstacles.clear();
	buddies.clear();
	foresightDots.clear();

	reducedCollectibleSpawnRange = 0.f;
	force = 1.f;
	spinMultiplier = 1.f;
	energyRangeModifier = 0.f;
	hasDashUpgrade = false;
	isDashing = false;
	dashTimer = 0.1f;
	dashCooldown = 0.f;
	dashSpeedMultiplier = 1.f;
	isNeverExhausted = false;
	canMoveUpAndDown = false;
	hasForesight = false;
}

void Paddle::SetPosition(const sf::Vector2f& newPosition)
{
	body.setPosition(newPosition);
}

void Paddle::UpdateEnergy(int energyTake)
{
	if (energyTake < 0 && isNeverExhausted) return;

	currentEnergy = std::clamp(currentEnergy + energyTake, 0, initialEnergy);

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
	float clampedX = std::clamp(body.getPosition().x, halfWidth, windowWidth - halfWidth);

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
	float obstacleY = screenPosition == PaddleScreenPosition::Top
		? windowHeight / 2.f - obstacleHeight / 2.f - 2.f
		: windowHeight / 2.f + obstacleHeight / 2.f + 2.f;

	sf::Color outlineColor = screenPosition == PaddleScreenPosition::Top
		? sf::Color::Red
		: sf::Color::Green;

	sf::RectangleShape obstacle({ obstacleWidth, obstacleHeight });
	obstacle.setPosition({ obstacleX, obstacleY });
	obstacle.setOutlineThickness(2.f);
	obstacle.setOrigin(obstacle.getGeometricCenter());
	obstacle.setFillColor(sf::Color(100, 100, 100));
	obstacle.setOutlineColor(outlineColor);

	obstacles.push_back(obstacle);
}

void Paddle::DisableExhaustion()
{
	isNeverExhausted = true;
}

void Paddle::EnableUpAndDownMovement()
{
	canMoveUpAndDown = true;
}

void Paddle::AddBuddy()
{
	float buddyYPos = body.getPosition().y;
	float buddyXPos = windowWidth / 2.f;

	std::unique_ptr<Buddy> newBuddy = std::make_unique<Buddy>(
		sf::Vector2f({ 80.f, 10.f }),
		this->screenPosition,
		sf::Vector2f({ buddyXPos, buddyYPos }),
		sf::Color(255, 255, 255, 150),
		initialSpeed / 2.f,
		windowWidth, windowHeight, initialEnergy);

	buddies.push_back(std::move(newBuddy));
}

void Paddle::EnableForesight()
{
	hasForesight = true;
}

void Paddle::ComputeForesight(const Ball& ball, int windowWidth, int windowHeight)
{
	foresightDots.clear();

	const float dotSpacing = 30.f;
	const float dotRadius = 3.f;

	sf::Vector2f direction({ ball.GetHorizontalDirection(), ball.GetVerticalDirection() });
	sf::Vector2f position(ball.GetBody().getPosition());

	float targetY = direction.y < 0 ? 0 : windowHeight;

	while ((direction.y < 0 && position.y > targetY) || (direction.y > 0 && position.y < targetY))
	{
		position.x += direction.x * dotSpacing;
		position.y += direction.y * dotSpacing;

		if (position.x + ball.GetCurrentRadius() >= windowWidth)
		{
			position.x = windowWidth - ball.GetCurrentRadius();
			direction.x *= -1;
		}
		else if (position.x - ball.GetCurrentRadius() <= 0)
		{
			position.x = ball.GetCurrentRadius();
			direction.x *= -1;
		}

		sf::CircleShape dot(dotRadius);
		dot.setOrigin(dot.getGeometricCenter());
		dot.setPosition({ position.x, position.y });
		dot.setFillColor(sf::Color(255, 255, 255, 120));

		foresightDots.push_back(dot);
	}
}

void Paddle::TrimForesight(float ballY, float verticalDirection)
{
	while (!foresightDots.empty())
	{
		float dotY = foresightDots.front().getPosition().y;

		bool isPassed = verticalDirection > 0
			? dotY < ballY
			: dotY > ballY;

		if (!isPassed) break;

		foresightDots.pop_front();
	}
}

void Paddle::DrawForesight(sf::RenderTarget& target) const
{
	if (foresightDots.size() == 0) return;

	for (const auto& dot : foresightDots)
		target.draw(dot);
}

// Getters:

float Paddle::GetCurrentSpeed() const
{
	return currentSpeed;
}

float Paddle::GetInitialSpeed() const
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

int Paddle::GetCollectedEnergy() const
{
	return energyCollected;
}

sf::FloatRect Paddle::GetGlobalBounds() const
{
	return body.getGlobalBounds();
}

PaddleScreenPosition Paddle::GetScreenPosition() const
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

const std::vector<std::unique_ptr<Paddle>>& Paddle::GetBuddies() const
{
	return buddies;
}

bool Paddle::HasForesight() const
{
	return hasForesight;
}