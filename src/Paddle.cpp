#include "Paddle.h"
#include "Buddy.h"

Paddle::Paddle(const sf::Vector2f& size, const PaddleScreenPosition screenPos, const sf::Vector2f& startPosition,
	const sf::Color& initialColor, float speed, int windowWidth, int windowHeight, int initialEnergy)
	: INITIAL_SPEED(speed), currentSpeed(speed), horizontalDirection(0), verticalDirection(0), SCREEN_POSITION(screenPos),
	WINDOW_WIDTH(windowWidth), WINDOW_HEIGHT(windowHeight), INITIAL_ENERGY(initialEnergy), currentEnergy(initialEnergy),
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
	currentSpeed = INITIAL_SPEED;
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
	dashTimer = DASH_DURATION;
	dashCooldown = 0.f;
	dashSpeedMultiplier = 1.f;
	isNeverExhausted = false;
	canMoveUpAndDown = false;
	hasForesight = false;
	curvaturePower = 0.f;
}

void Paddle::SetPosition(const sf::Vector2f& newPosition)
{
	body.setPosition(newPosition);
}

void Paddle::UpdateEnergy(int energyTake)
{
	if (energyTake < 0 && isNeverExhausted) return;

	currentEnergy = std::clamp(currentEnergy + energyTake, 0, INITIAL_ENERGY);

	float ratio = static_cast<float>(currentEnergy) / INITIAL_ENERGY;

	currentSpeed = INITIAL_SPEED * ratio;
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

bool Paddle::HasUniqueUpgrade(UpgradeType type) const
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
	float clampedX = std::clamp(body.getPosition().x, halfWidth, WINDOW_WIDTH - halfWidth);

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

void Paddle::PlaceObstacle(float obstacleWidth, float obstacleHeight, const sf::Color outlineColor)
{
	std::uniform_real_distribution<float> random(obstacleWidth / 2.f, WINDOW_WIDTH - obstacleWidth / 2.f);
	float obstacleX = random(rng);
	bool moved = true;

	while (moved)
	{
		moved = false;

		for (const auto& obs : obstacles)
		{
			float existingLeft  = obs.getPosition().x - obstacleWidth / 2.f;
			float existingRight = obs.getPosition().x + obstacleWidth / 2.f;
			float newLeft       = obstacleX - obstacleWidth / 2.f;
			float newRight      = obstacleX + obstacleWidth / 2.f;

			if (newLeft < existingRight && newRight > existingLeft)
			{
				// Obstacle will be placed left side
				if (obstacleX < obs.getPosition().x)
					obstacleX = existingLeft - obstacleWidth / 2.f;
				else
					obstacleX = existingRight + obstacleWidth / 2.f;

				moved = true;
				break;
			}
		}
	}
	obstacleX = std::clamp(obstacleX, obstacleWidth / 2.f, WINDOW_WIDTH - obstacleWidth / 2.f);

	sf::RectangleShape obstacle({ obstacleWidth, obstacleHeight });
	obstacle.setOrigin(obstacle.getGeometricCenter());

	float obstacleY = SCREEN_POSITION == PaddleScreenPosition::Top
		? WINDOW_HEIGHT / 2.f - obstacleHeight / 2.f - 1.f
		: WINDOW_HEIGHT / 2.f + obstacleHeight / 2.f + 1.f;

	obstacle.setPosition({ obstacleX, obstacleY });
	obstacle.setFillColor(sf::Color(100, 100, 100));
	obstacle.setOutlineThickness(1.f);
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
	float buddyXPos = WINDOW_WIDTH / 2.f;

	std::unique_ptr<Buddy> newBuddy = std::make_unique<Buddy>(
		sf::Vector2f({ BUDDY_WIDTH, BUDDY_HEIGHT }),
		this->SCREEN_POSITION,
		sf::Vector2f({ buddyXPos, buddyYPos }),
		sf::Color(255, 255, 255, 150),
		INITIAL_SPEED / 2.f,
		WINDOW_WIDTH, WINDOW_HEIGHT, INITIAL_ENERGY);

	buddies.push_back(std::move(newBuddy));
}

void Paddle::EnableForesight()
{
	hasForesight = true;
}

void Paddle::ComputeForesight(const Ball& ball)
{
	foresightDots.clear();

	sf::Vector2f direction({ ball.GetHorizontalDirection(), ball.GetVerticalDirection() });
	sf::Vector2f position(ball.GetBody().getPosition());

	float targetY = direction.y < 0 ? 0 : WINDOW_HEIGHT;

	while ((direction.y < 0 && position.y > targetY) || (direction.y > 0 && position.y < targetY))
	{
		position.x += direction.x * FORESIGHT_DOT_SPACING;
		position.y += direction.y * FORESIGHT_DOT_SPACING;

		if (position.x + ball.GetCurrentRadius() >= WINDOW_WIDTH)
		{
			position.x = WINDOW_WIDTH - ball.GetCurrentRadius();
			direction.x *= -1;
		}
		else if (position.x - ball.GetCurrentRadius() <= 0)
		{
			position.x = ball.GetCurrentRadius();
			direction.x *= -1;
		}

		sf::CircleShape dot(FORESIGHT_DOT_RADIUS);
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
	if (foresightDots.empty()) return;

	for (const auto& dot : foresightDots)
		target.draw(dot);
}

void Paddle::AddCurvaturePower(float power)
{
	curvaturePower += power;
}

// Getters:

float Paddle::GetCurrentSpeed() const
{
	return currentSpeed;
}

float Paddle::GetInitialSpeed() const
{
	return INITIAL_SPEED;
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
	return SCREEN_POSITION;
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

float Paddle::GetCurvaturePower() const
{
	return curvaturePower;
}

float Paddle::GetDashCooldownRatio() const
{
	if (!hasDashUpgrade) return 1.f;
	return 1.f - std::clamp(dashCooldown / DASH_COOLDOWN_DURATION, 0.f, 1.f);
}