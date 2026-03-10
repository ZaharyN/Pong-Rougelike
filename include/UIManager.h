#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
#include "Types.h"

class UIManager
{
	const float BUTTON_WIDTH = 300.f;
	const float BUTTON_HEIGHT = 100.f;
	const float OUTLINE_THICKNESS = 5.f;

	const unsigned int windowWidth;
	sf::Font font;

	sf::RectangleShape startButton;
	sf::RectangleShape exitButton;
	std::optional<sf::Text> startButtonText;
	std::optional<sf::Text> exitButtonText;

	sf::RectangleShape onePlayerGameButton;
	sf::RectangleShape twoPlayerGameButton;
	std::optional<sf::Text> onePlayerButtonText;
	std::optional<sf::Text> twoPlayerButtonText;

	void CreateButton(sf::RectangleShape& button, float yPosition);
	void CreateButtonText(std::optional<sf::Text>& text, const std::string& value, int size, const sf::RectangleShape& parent);
	void OnHoverEffect(sf::RectangleShape& button, std::optional<sf::Text>& text);
	void ResetHoverEffect(sf::RectangleShape& button, std::optional<sf::Text>& text);

public:
	UIManager(const unsigned int windowWidth);
	void InitializeStartMenu();
	void InitializeSelectModeMenu();

	void Update(GameState state, const sf::RenderWindow& gameWindow);
	void Render(GameState state, sf::RenderWindow& gameWindow);

	bool IsStartClicked(const sf::Vector2f& mousePos) const
	{
		return startButton.getGlobalBounds().contains(mousePos);
	}

	bool IsExitClicked(const sf::Vector2f& mousePos) const
	{
		return exitButton.getGlobalBounds().contains(mousePos);
	}

	bool IsOnePlayerClicked(const sf::Vector2f& mousePos) const
	{
		return onePlayerGameButton.getGlobalBounds().contains(mousePos);
	}

	bool IsTwoPlayerClicked(const sf::Vector2f& mousePos) const
	{
		return twoPlayerGameButton.getGlobalBounds().contains(mousePos);
	}
};