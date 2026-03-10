#pragma once
#include <SFML/Graphics.hpp>
#include "Types.h"
#include <iostream>

class Paddle
{
protected:
	sf::RectangleShape body;
	sf::Color color;
	PaddleScreenPosition screenPosition;
	const float initialSpeed;
	float currentSpeed;
	int initialEnergy;
	int currentEnergy;
	int horizontalDirection;
	int windowWidth;

public:
	Paddle(const sf::Vector2f& size,const PaddleScreenPosition screenPos, const sf::Vector2f& startPosition, 
		const sf::Color& initialColor, float speed, int windowWidth, int initialEnergy);

	virtual void Update(float deltaT) = 0;

	virtual void Draw(sf::RenderTarget& target);

	virtual void Reset();

	void SetPosition(const sf::Vector2f& newPosition);

	void ReduceEnergy(int energyTaken);

	void ChangeColorFromRation(float ratio);

	virtual ~Paddle() = default;

	const float GetCurrentSpeed() const;

	const float GetInitialSpeed() const;

	int GetXDirection() const;

	const sf::RectangleShape& GetBody() const;

	const PaddleScreenPosition GetScreenPosition() const;

	sf::FloatRect GetGlobalBounds() const;
};
