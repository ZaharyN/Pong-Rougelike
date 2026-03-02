#pragma once
#include <SFML/Graphics.hpp>

enum PaddleScreenPosition
{
	Top = 1,
	Bottom = 2,
};

class Paddle
{
protected:
	sf::RectangleShape body;
	sf::Color color;
	PaddleScreenPosition screenPosition;
	const float initialSpeed;
	float currentSpeed;
	int horizontalDirection;

public:
	Paddle(const sf::Vector2f& size,const PaddleScreenPosition screenPos, const sf::Vector2f& startPosition, const sf::Color& initialColor, const float& speed);

	virtual void Update(const float& deltaT) = 0;

	virtual void Draw(sf::RenderTarget& target);

	virtual void Reset();

	void SetPosition(const sf::Vector2f& newPosition);

	~Paddle();

	const float GetCurrentSpeed() const;

	const float GetInitialSpeed() const;

	const sf::RectangleShape& GetBody() const;
};
