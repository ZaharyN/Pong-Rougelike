#pragma once
#include "SFML/Graphics.hpp"
#include "Types.h"
#include "UpgradeCard.h"
#include <iostream>
#include <string>
#include <sstream>

class UIManager
{
	static constexpr float BUTTON_WIDTH = 300.f;
	static constexpr float BUTTON_HEIGHT = 100.f;
	static constexpr float OUTLINE_THICKNESS = 5.f;

	static constexpr float UPGRADE_CARD_WIDTH = 300.f;
	static constexpr float UPGRADE_CARD_HEIGHT = 400.f;
	static constexpr float UPGRADE_CARD_GAP = 75.f;
	static constexpr float UPGRADE_CARD_TITLE_HEGITH = 60.f;

	static constexpr unsigned int FONT_SIZE_BUTTON = 40;
	static constexpr unsigned int FONT_SIZE_CARD_DESCRIPTION = 20;

	static constexpr float SCORE_TEXT_OFFSET = 20.f;
	static constexpr float ON_HOVER_SCALE = 1.05f;
 
	static constexpr float OVERLAY_DURATION = 0.2f;
	static constexpr float OVERLAY_OUTLINE_THICKNESS = -25.f;
	static constexpr float OVERLAY_MAX_ALPHA = 150.f;
	static constexpr float OVERLAY_HEIGHT = 0.f;
	const float OVERLAY_WIDTH;

	const unsigned int WINDOW_WIDTH;
	const unsigned int WINDOW_HEIGHT;

	sf::Font font;

	sf::RectangleShape startButton;
	sf::RectangleShape exitButton;
	std::optional<sf::Text> startButtonText;
	std::optional<sf::Text> exitButtonText;

	sf::RectangleShape onePlayerGameButton;
	sf::RectangleShape twoPlayerGameButton;
	std::optional<sf::Text> onePlayerButtonText;
	std::optional<sf::Text> twoPlayerButtonText;

	std::array<UpgradeCard, 3> upgradeCards;

	sf::RectangleShape playerPickingBox;
	std::optional<sf::Text> playerPickingText;

	std::optional<sf::Text> player1ScoreText;
	std::optional<sf::Text> player2ScoreText;

	std::optional<sf::Text> gameOverText;
	std::optional<sf::Text> winnerText;

	sf::RectangleShape damageOverlay;
	float damagerOverlayTimer = 0.0f;
	bool isOverlayActive = false;

	void CreateButton(sf::RectangleShape& button, float yPosition);
	void CreateButtonText(std::optional<sf::Text>& text, const std::string& value, int size, const sf::RectangleShape& parent);
	void OnHoverEffect(sf::RectangleShape& button, std::optional<sf::Text>& text);
	void ResetHoverEffect(sf::RectangleShape& button, std::optional<sf::Text>& text);
	sf::Color GetColorFromRarity(UpgradeRarity rarity);
	std::string WrapText(const std::string& str, float maxWidth, unsigned int fontSize);

public:
	UIManager(const unsigned int windowWidth, const unsigned int windowHeight);

	void InitializeStartMenu();
	void InitializeSelectModeMenu();
	void InitializeUpgradeMenu();
	void InitializeScoreTexts();
	void InitializeGameOverScreen(const std::string_view winnerName);
	void InitializeOverlay();

	void Update(float deltaT, GameState state, const sf::RenderWindow& gameWindow);
	void Draw(GameState state, sf::RenderWindow& gameWindow);

	int GetClickedCardIndex(const sf::Vector2f& mousePos) const;
	ClickTarget GetClickedTarget(const sf::Vector2f& mousePos, GameState gameState);
	void UpdateScores(int player1Score, int player2Score);

	void ShowRandomUpgrades(const std::vector<Upgrade>& upgrades, const std::string_view playerName);

	void TriggerDamageOverlay(PaddleScreenPosition screenPosition);
};