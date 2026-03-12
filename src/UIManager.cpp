#include "UIManager.h"

UIManager::UIManager(const unsigned int windowWidth)
	: windowWidth(windowWidth)
{
	if (!font.openFromFile("Assets/Fonts/game_font.ttf"))
	{
		std::cout << "Error: Could not find font file in assets folder!" << std::endl;
	}
	InitializeStartMenu();
	InitializeSelectModeMenu();
}

void UIManager::InitializeStartMenu()
{
	startButton = sf::RectangleShape({ BUTTON_WIDTH, BUTTON_HEIGHT });
	CreateButton(startButton, 200);

	exitButton = sf::RectangleShape({ BUTTON_WIDTH, BUTTON_HEIGHT });
	CreateButton(exitButton, startButton.getPosition().y + BUTTON_HEIGHT / 2.0 + 100);

	CreateButtonText(startButtonText, "START GAME", 40, startButton);
	CreateButtonText(exitButtonText, "EXIT", 40, exitButton);
}

void UIManager::Update(GameState state, const sf::RenderWindow& gameWindow)
{
	sf::Vector2i pixelPosition = sf::Mouse::getPosition(gameWindow);
	sf::Vector2f mouseWorldPos = gameWindow.mapPixelToCoords(pixelPosition);

	if (state == GameState::Menu)
	{
		if (startButton.getGlobalBounds().contains(mouseWorldPos))
			OnHoverEffect(startButton, startButtonText);
		else
			ResetHoverEffect(startButton, startButtonText);

		if (exitButton.getGlobalBounds().contains(mouseWorldPos))
			OnHoverEffect(exitButton, exitButtonText);
		else
			ResetHoverEffect(exitButton, exitButtonText);
	}
	else if (state == GameState::ModeSelect)
	{
		if (onePlayerGameButton.getGlobalBounds().contains(mouseWorldPos))
			OnHoverEffect(onePlayerGameButton, onePlayerButtonText);
		else
			ResetHoverEffect(onePlayerGameButton, onePlayerButtonText);

		if (twoPlayerGameButton.getGlobalBounds().contains(mouseWorldPos))
			OnHoverEffect(twoPlayerGameButton, twoPlayerButtonText);
		else
			ResetHoverEffect(twoPlayerGameButton, twoPlayerButtonText);
	}
}

void UIManager::Render(GameState state, sf::RenderWindow& gameWindow)
{
	if (state == GameState::Menu)
	{
		gameWindow.draw(startButton);
		gameWindow.draw(exitButton);

		gameWindow.draw(*startButtonText);
		gameWindow.draw(*exitButtonText);
	}
	else if (state == GameState::ModeSelect)
	{
		gameWindow.draw(onePlayerGameButton);
		gameWindow.draw(twoPlayerGameButton);

		gameWindow.draw(*onePlayerButtonText);
		gameWindow.draw(*twoPlayerButtonText);
	}
}

void UIManager::ShowRandomUpgrades(const std::vector<Upgrade>& upgrades)
{
	for (size_t i = 0; i < upgrades.size(); i++)
	{

	}
}

void UIManager::InitializeSelectModeMenu()
{
	onePlayerGameButton = sf::RectangleShape({ BUTTON_WIDTH * 2, BUTTON_HEIGHT });
	CreateButton(onePlayerGameButton, 200);

	twoPlayerGameButton = sf::RectangleShape({ BUTTON_WIDTH * 2, BUTTON_HEIGHT });
	CreateButton(twoPlayerGameButton, onePlayerGameButton.getPosition().y + BUTTON_HEIGHT / 2.0 + 100);

	CreateButtonText(onePlayerButtonText, "ONE PLAYER GAME", 40, onePlayerGameButton);
	CreateButtonText(twoPlayerButtonText, "TWO PLAYER GAME", 40, twoPlayerGameButton);
}

void UIManager::CreateButton(sf::RectangleShape& button, float yPosition)
{
	button.setOrigin(button.getGeometricCenter());
	button.setPosition({ windowWidth / 2.f, yPosition });
	button.setFillColor(sf::Color::Black);
	button.setOutlineThickness(OUTLINE_THICKNESS);
	button.setOutlineColor(sf::Color::White);
}

void UIManager::CreateButtonText(std::optional<sf::Text>& text, const std::string& value, int size, const sf::RectangleShape& parent)
{
	text.emplace(font);
	text->setString(std::string(value));
	text->setCharacterSize(size);
	text->setFillColor(sf::Color::White);

	sf::FloatRect localBounds = text->getLocalBounds();
	text->setOrigin({ localBounds.size.x / 2, localBounds.size.y});
	text->setPosition(parent.getPosition());
}

void UIManager::OnHoverEffect(sf::RectangleShape& button, std::optional<sf::Text>& text)
{
	button.setFillColor(sf::Color::White);
	button.setScale({ 1.05f, 1.05f });
	text->setFillColor(sf::Color::Black);
}

void UIManager::ResetHoverEffect(sf::RectangleShape& button, std::optional<sf::Text>& text)
{
	button.setFillColor(sf::Color::Black);
	text->setFillColor(sf::Color::White);
	button.setScale({ 1.0f, 1.0f });
}