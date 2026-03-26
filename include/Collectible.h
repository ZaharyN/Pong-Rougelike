#pragma once
#include "SFML/Graphics.hpp"

class Collectible
{
	sf::Sprite sprite;

public:
	Collectible(const sf::Vector2f& screenPos, const sf::Texture& texture);

	void Draw(sf::RenderTarget& gameWindow);
	sf::FloatRect GetBounds() const;
};