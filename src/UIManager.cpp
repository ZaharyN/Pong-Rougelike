#include "UIManager.h"

UIManager::UIManager(const unsigned int windowWidth, const unsigned int windowHeight)
	: WINDOW_WIDTH(windowWidth), WINDOW_HEIGHT(windowHeight)
{
	if (!font.openFromFile("Assets/Fonts/game_font.ttf"))
	{
		std::cout << "Error: Could not find font file in assets folder!" << std::endl;
	}
	InitializeStartMenu();
	InitializeSelectModeMenu();
	InitializeUpgradeMenu();
	InitializeScoreTexts();
}

void UIManager::InitializeStartMenu()
{
	startButton = sf::RectangleShape({ BUTTON_WIDTH, BUTTON_HEIGHT });
	CreateButton(startButton, 200);

	exitButton = sf::RectangleShape({ BUTTON_WIDTH, BUTTON_HEIGHT });
	CreateButton(exitButton, startButton.getPosition().y + BUTTON_HEIGHT / 2.0 + 100);

	CreateButtonText(startButtonText, "START GAME", FONT_SIZE_BUTTON, startButton);
	CreateButtonText(exitButtonText, "EXIT", FONT_SIZE_BUTTON, exitButton);
}

void UIManager::InitializeSelectModeMenu()
{
	onePlayerGameButton = sf::RectangleShape({ BUTTON_WIDTH * 2, BUTTON_HEIGHT });
	CreateButton(onePlayerGameButton, 200);

	twoPlayerGameButton = sf::RectangleShape({ BUTTON_WIDTH * 2, BUTTON_HEIGHT });
	CreateButton(twoPlayerGameButton, onePlayerGameButton.getPosition().y + BUTTON_HEIGHT / 2.0 + 100);

	CreateButtonText(onePlayerButtonText, "ONE PLAYER GAME", FONT_SIZE_BUTTON, onePlayerGameButton);
	CreateButtonText(twoPlayerButtonText, "TWO PLAYER GAME", FONT_SIZE_BUTTON, twoPlayerGameButton);
}

void UIManager::InitializeUpgradeMenu()
{
	float posY = WINDOW_HEIGHT / 2.f;

	for (int i = 0; i < 3; i++)
	{
		float posX = (i + 1) * UPGRADE_CARD_GAP + i * UPGRADE_CARD_WIDTH + UPGRADE_CARD_WIDTH / 2.f;
		upgradeCards[i].Initialize(posX, posY, UPGRADE_CARD_WIDTH, UPGRADE_CARD_HEIGHT, font);
	}

	playerPickingBox = sf::RectangleShape({ BUTTON_WIDTH, BUTTON_HEIGHT / 2.f });
	CreateButton(playerPickingBox, BUTTON_HEIGHT / 4.f + 10);
}

void UIManager::InitializeScoreTexts()
{
	player1ScoreText.emplace(font, "0", FONT_SIZE_BUTTON);
	player1ScoreText->setOrigin({
		player1ScoreText->getLocalBounds().size.x / 2.f,
		player1ScoreText->getLocalBounds().size.y / 2.f
		});
	player1ScoreText->setPosition({
		WINDOW_WIDTH - player1ScoreText->getGlobalBounds().size.x / 2.f,
		WINDOW_HEIGHT / 2.f + player1ScoreText->getGlobalBounds().size.y / 2.f + SCORE_TEXT_OFFSET
		});

	player2ScoreText.emplace(font, "0", FONT_SIZE_BUTTON);
	player2ScoreText->setOrigin({
		player2ScoreText->getLocalBounds().size.x / 2.f,
		player2ScoreText->getLocalBounds().size.y / 2.f
		});
	player2ScoreText->setPosition({
		WINDOW_WIDTH - player2ScoreText->getGlobalBounds().size.x / 2.f,
		WINDOW_HEIGHT / 2.f - player2ScoreText->getGlobalBounds().size.y / 2.f - SCORE_TEXT_OFFSET
		});
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
	else if (state == GameState::UpgradeSelect)
	{
		for (auto& upgradeCard : upgradeCards)
		{
			upgradeCard.SetHovered(upgradeCard.Contains(mouseWorldPos));
		}
	}
}

void UIManager::Draw(GameState state, sf::RenderWindow& gameWindow)
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
	else if (state == GameState::UpgradeSelect)
	{
		for (const auto& upgradeCard : upgradeCards)
			upgradeCard.Draw(gameWindow);

		gameWindow.draw(playerPickingBox);
		if (playerPickingText) gameWindow.draw(*playerPickingText);
	}
	else if (state == GameState::Playing)
	{
		if (player1ScoreText) gameWindow.draw(*player1ScoreText);
		if (player2ScoreText) gameWindow.draw(*player2ScoreText);
	}
	else if (state == GameState::GameOver)
	{
		if (gameOverText) gameWindow.draw(*gameOverText);
		if (winnerText) gameWindow.draw(*winnerText);
	}
}

void UIManager::UpdateScores(int player1Score, int player2Score)
{
	auto updateText = [&](std::optional<sf::Text>& text, int score, float posY)
		{
			text.emplace(font, std::to_string(score), FONT_SIZE_BUTTON);
			text->setFillColor(sf::Color::White);
			sf::FloatRect bounds = text->getLocalBounds();
			text->setOrigin({ bounds.size.x / 2.f, bounds.size.y });
			text->setPosition({
				WINDOW_WIDTH - bounds.size.x - SCORE_TEXT_OFFSET,
				posY
			});
		};

	updateText(player1ScoreText, player1Score, WINDOW_HEIGHT / 2.f + SCORE_TEXT_OFFSET);
	updateText(player2ScoreText, player2Score, WINDOW_HEIGHT / 2.f - SCORE_TEXT_OFFSET);
}

void UIManager::ShowRandomUpgrades(const std::vector<Upgrade>& upgrades, const std::string_view playerName)
{
	for (int i = 0; i < 3; i++)
	{
		sf::Color rarityColor = GetColorFromRarity(upgrades[i].rarity);
		std::string wrapped = WrapText(upgrades[i].description, UPGRADE_CARD_WIDTH - 20.f, FONT_SIZE_CARD_DESCRIPTION);

		upgradeCards[i].SetContent(upgrades[i], font, UPGRADE_CARD_WIDTH,
			FONT_SIZE_BUTTON, FONT_SIZE_CARD_DESCRIPTION,
			wrapped, rarityColor);
	}

	CreateButtonText(playerPickingText, std::string(playerName), FONT_SIZE_BUTTON, playerPickingBox);
}

void UIManager::InitializeGameOverScreen(const std::string_view winnerName)
{
	gameOverText.emplace(font, "GAME OVER", 60);
	gameOverText->setFillColor(sf::Color::White);
	sf::FloatRect goBounds = gameOverText->getLocalBounds();
	gameOverText->setOrigin({ goBounds.size.x / 2.f, goBounds.size.y });
	gameOverText->setPosition({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f - 60.f });

	winnerText.emplace(font, std::string(winnerName) + " WINS!", FONT_SIZE_BUTTON);
	winnerText->setFillColor(sf::Color::Yellow);
	sf::FloatRect wBounds = winnerText->getLocalBounds();
	winnerText->setOrigin({ wBounds.size.x / 2.f, wBounds.size.y });
	winnerText->setPosition({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f + 20.f });
}

int UIManager::GetClickedCardIndex(const sf::Vector2f& mousePos) const
{
	for (int i = 0; i < 3; i++)
	{
		if (upgradeCards[i].Contains(mousePos))
			return i;
	}
	return -1;
}

void UIManager::CreateButton(sf::RectangleShape& button, float yPosition)
{
	button.setOrigin(button.getGeometricCenter());
	button.setPosition({ WINDOW_WIDTH / 2.f, yPosition });
	button.setFillColor(sf::Color::Black);
	button.setOutlineThickness(OUTLINE_THICKNESS);
	button.setOutlineColor(sf::Color::White);
}

void UIManager::CreateButtonText(std::optional<sf::Text>& text, const std::string& value, int size, const sf::RectangleShape& parent)
{
	text.emplace(font);
	text->setString(value);
	text->setCharacterSize(size);
	text->setFillColor(sf::Color::White);

	sf::FloatRect localBounds = text->getLocalBounds();
	text->setOrigin({ localBounds.size.x / 2, localBounds.size.y });
	text->setPosition(parent.getPosition());
}

void UIManager::OnHoverEffect(sf::RectangleShape& button, std::optional<sf::Text>& text)
{
	button.setFillColor(sf::Color::White);
	button.setScale({ ON_HOVER_SCALE, ON_HOVER_SCALE });
	text->setFillColor(sf::Color::Black);
}

void UIManager::ResetHoverEffect(sf::RectangleShape& button, std::optional<sf::Text>& text)
{
	button.setFillColor(sf::Color::Black);
	text->setFillColor(sf::Color::White);
	button.setScale({ 1.0f, 1.0f });
}

sf::Color UIManager::GetColorFromRarity(UpgradeRarity rarity)
{
	switch (rarity)
	{
	case UpgradeRarity::Common:
		return sf::Color::White;
	case UpgradeRarity::Uncommon:
		return sf::Color::Blue;
	case UpgradeRarity::Legendary:
		return sf::Color::Yellow;
	}
	return sf::Color::White;
}

ClickTarget UIManager::GetClickedTarget(const sf::Vector2f& mousePos, GameState gameState)
{
	switch (gameState)
	{
	case GameState::Menu:
		if (startButton.getGlobalBounds().contains(mousePos))
			return ClickTarget::StartButton;
		if (exitButton.getGlobalBounds().contains(mousePos))
			return ClickTarget::ExitButton;
		return ClickTarget::None;

	case GameState::ModeSelect:
		if (onePlayerGameButton.getGlobalBounds().contains(mousePos))
			return ClickTarget::OnePlayerOption;
		if (twoPlayerGameButton.getGlobalBounds().contains(mousePos))
			return ClickTarget::TwoPlayerOption;
		return ClickTarget::None;

	case GameState::UpgradeSelect:
		for (int i = 0; i < upgradeCards.size(); i++)
		{
			if (upgradeCards[i].Contains(mousePos))
			{
				return static_cast<ClickTarget>
					(static_cast<int>(ClickTarget::UpgradeCard1) + i);
			}
		}
		return ClickTarget::None;

	default:
		return ClickTarget::None;
	}
}

std::string UIManager::WrapText(const std::string& str, float maxWidth, unsigned int fontSize)
{
	std::string result;
	std::string line;
	std::stringstream ss(str);
	std::string word;

	sf::Text tempText(font, "", fontSize);

	while (ss >> word)
	{
		std::string testLine = line + (line.empty() ? "" : " ") + word;
		tempText.setString(testLine);

		if (tempText.getGlobalBounds().size.x > maxWidth)
		{
			result += line + "\n";
			line = word;
		}
		else
		{
			line = testLine;
		}
	}
	return result + line;
}
