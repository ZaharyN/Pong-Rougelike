#include "Collectible.h"

Collectible::Collectible(const sf::Vector2f& screenPos, const sf::Texture& texture)
	: sprite(texture)
{
	sf::FloatRect bounds = sprite.getLocalBounds();
	sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
	sprite.setPosition(screenPos);
}

sf::FloatRect Collectible::GetBounds() const
{
	return this->sprite.getGlobalBounds();
}

void Collectible::Draw(sf::RenderTarget& window)
{
	window.draw(sprite);
}