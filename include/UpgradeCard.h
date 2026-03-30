#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include "Types.h"

class UpgradeCard
{
	static constexpr float TITLE_HEIGHT = 60.F;
	static constexpr float OUTLINE_THICKNESS = 5.f;

	sf::RectangleShape card;
	sf::RectangleShape titleBar;
	std::optional<sf::Text> titleText;
	std::optional<sf::Text> descriptionText;
	bool isHovered;

public:
	void Initialize(float posX, float posY, float cardWidth, float cardHeight, const sf::Font& font);

	void SetContent(const Upgrade& upgrade, const sf::Font& font, float cardWidth, unsigned int titleFontSize,
		unsigned int descFontSize, const std::string& wrappedDescription, sf::Color rarityColor);

	void SetHovered(bool hovered);

	void Draw(sf::RenderTarget& target) const;

	bool Contains(const sf::Vector2f& mousePos) const;
};