#pragma once
#include "SFML/Graphics.hpp"
#include "Types.h"
#include <iostream>
#include <string>
#include <sstream>

class UIManager
{
	const float BUTTON_WIDTH = 300.f;
	const float BUTTON_HEIGHT = 100.f;
	const float OUTLINE_THICKNESS = 5.f;
	const float UPGRADE_CARD_WIDTH = 300.f;
	const float UPGRADE_CARD_HEIGHT = 400.f;
	const float UPGRADE_CARD_GAP = 75.f;

	const unsigned int windowWidth;
	const unsigned int windowHeight;
	sf::Font font;

	sf::RectangleShape startButton;
	sf::RectangleShape exitButton;
	std::optional<sf::Text> startButtonText;
	std::optional<sf::Text> exitButtonText;

	sf::RectangleShape onePlayerGameButton;
	sf::RectangleShape twoPlayerGameButton;
	std::optional<sf::Text> onePlayerButtonText;
	std::optional<sf::Text> twoPlayerButtonText;

	// Upgrade card UI
	sf::RectangleShape upgradeCard1;
	sf::RectangleShape upgradeCard2;
	sf::RectangleShape upgradeCard3;

	sf::RectangleShape card1Title;
	sf::RectangleShape card2Title;
	sf::RectangleShape card3Title;
	std::optional<sf::Text> card1TitleText;
	std::optional<sf::Text> card2TitleText;
	std::optional<sf::Text> card3TitleText;

	std::optional<sf::Text> card1DescriptionText;
	std::optional<sf::Text> card2DescriptionText;
	std::optional<sf::Text> card3DescriptionText;

	void CreateButton(sf::RectangleShape& button, float yPosition);

	void CreateButtonText(std::optional<sf::Text>& text, const std::string& value, int size, const sf::RectangleShape& parent);

	void OnHoverEffect(sf::RectangleShape& button, std::optional<sf::Text>& text);

	void ResetHoverEffect(sf::RectangleShape& button, std::optional<sf::Text>& text);
	
	void OnCardHoverEffect(sf::RectangleShape& card, sf::RectangleShape& title, 
		std::optional<sf::Text>& titleText, std::optional<sf::Text>& description);
	
	void ResetCardHoverEffect(sf::RectangleShape& card, sf::RectangleShape& title,
		std::optional<sf::Text>& titleText, std::optional<sf::Text>& description);

	sf::Color GetColorFromRarity(UpgradeRarity rarity);

	std::string WrapText(const std::string& str, float maxWidth, unsigned int fontSize);
public:
	UIManager(const unsigned int windowWidth, const unsigned int windowHeight);

	void InitializeStartMenu();

	void InitializeSelectModeMenu();

	void InitializeUpgradeMenu();

	void Update(GameState state, const sf::RenderWindow& gameWindow);

	void Draw(GameState state, sf::RenderWindow& gameWindow);

	void ShowRandomUpgrades(const std::vector<Upgrade>& upgrades);

	int GetClickedCardIndex(const sf::Vector2f& mousePos) const;

	ClickTarget GetClickedTarget(const sf::Vector2f& mousePos, GameState gameState);
};