#include "Collectible.h"

Collectible::Collectible(const sf::Vector2f& screenPos, float width, 
	float height, const sf::Color& color)
	: position(screenPos), width(width), height(height)
{
	body = sf::RectangleShape(sf::Vector2f{ width, height });
	body.setOrigin(body.getGeometricCenter());
	body.setPosition(screenPos);
	body.setFillColor(color);
}

sf::FloatRect Collectible::GetBounds() const
{
	return this->body.getGlobalBounds();
}

void Collectible::Draw(sf::RenderTarget& window)
{
	window.draw(body);
}