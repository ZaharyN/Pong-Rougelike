#include "UpgradeCard.h"

void UpgradeCard::Initialize(float posX, float posY, float cardWidth, float cardHeight, const sf::Font& font)
{
	card = sf::RectangleShape({ cardWidth, cardHeight });
	card.setOrigin(card.getGeometricCenter());
	card.setPosition({ posX, posY });
	card.setFillColor(sf::Color::Black);
	card.setOutlineThickness(OUTLINE_THICKNESS);

	titleBar = sf::RectangleShape({ cardWidth, TITLE_HEIGHT });
	titleBar.setOrigin(titleBar.getGeometricCenter());
	titleBar.setPosition({ posX, posY - cardHeight / 2.f + titleBar.getGlobalBounds().size.y / 2.f });
	titleBar.setFillColor(sf::Color::White);
	titleBar.setOutlineThickness(OUTLINE_THICKNESS);
}

void UpgradeCard::SetContent(const Upgrade& upgrade, const sf::Font& font,
	float cardWidth, unsigned int titleFontSize, unsigned int descFontSize,
	const std::string& wrappedDescription, sf::Color rarityColor)
{
	card.setOutlineColor(rarityColor);
	titleBar.setOutlineColor(rarityColor);

	// Title text
	titleText.emplace(font, upgrade.title, titleFontSize);
	titleText->setFillColor(sf::Color::Black);
	sf::FloatRect titleBounds = titleText->getLocalBounds();
	titleText->setOrigin({ titleBounds.size.x / 2.f, titleBounds.size.y });
	titleText->setPosition(titleBar.getPosition());

	// Description text
	descriptionText.emplace(font, wrappedDescription, descFontSize);
	descriptionText->setFillColor(sf::Color::White);
	sf::FloatRect descBounds = descriptionText->getGlobalBounds();
	descriptionText->setOrigin({ descBounds.size.x / 2.f, descBounds.size.y });
	descriptionText->setPosition(card.getPosition());
}

void UpgradeCard::SetHovered(bool hovered)
{
	isHovered = hovered;
	card.setFillColor(hovered ? sf::Color::White : sf::Color::Black);
	titleBar.setFillColor(hovered ? sf::Color::White : sf::Color::Black);

	if (titleText)
		titleText->setFillColor(hovered ? sf::Color::Black : sf::Color::White);
	if (descriptionText)
		descriptionText->setFillColor(hovered ? sf::Color::Black : sf::Color::White);
}

void UpgradeCard::Draw(sf::RenderTarget& target) const
{
	target.draw(card);
	target.draw(titleBar);

	if (titleText)
		target.draw(*titleText);
	if (descriptionText)
		target.draw(*descriptionText);
}

bool UpgradeCard::Contains(const sf::Vector2f& mousePos) const
{
	return card.getGlobalBounds().contains(mousePos);
}