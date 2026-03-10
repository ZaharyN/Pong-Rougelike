#pragma once
#include "SFML/Graphics.hpp"

class Collectible
{
	sf::RectangleShape body;
	sf::Vector2f position;

	float width;
	float height;

public:
	Collectible(const sf::Vector2f& screenPos, float width, float height, const sf::Color& color);

	void Draw(sf::RenderTarget& gameWindow);
	sf::FloatRect GetBounds() const;
};