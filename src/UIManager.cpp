#include "UIManager.h"

UIManager::UIManager(const unsigned int windowWidth, const unsigned int windowHeight)
	: windowWidth(windowWidth), windowHeight(windowHeight)
{
	if (!font.openFromFile("Assets/Fonts/game_font.ttf"))
	{
		std::cout << "Error: Could not find font file in assets folder!" << std::endl;
	}
	InitializeStartMenu();
	InitializeSelectModeMenu();
	InitializeUpgradeMenu();
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

void UIManager::InitializeSelectModeMenu()
{
	onePlayerGameButton = sf::RectangleShape({ BUTTON_WIDTH * 2, BUTTON_HEIGHT });
	CreateButton(onePlayerGameButton, 200);

	twoPlayerGameButton = sf::RectangleShape({ BUTTON_WIDTH * 2, BUTTON_HEIGHT });
	CreateButton(twoPlayerGameButton, onePlayerGameButton.getPosition().y + BUTTON_HEIGHT / 2.0 + 100);

	CreateButtonText(onePlayerButtonText, "ONE PLAYER GAME", 40, onePlayerGameButton);
	CreateButtonText(twoPlayerButtonText, "TWO PLAYER GAME", 40, twoPlayerGameButton);
}

void UIManager::InitializeUpgradeMenu()
{
	upgradeCard1 = sf::RectangleShape({ UPGRADE_CARD_WIDTH, UPGRADE_CARD_HEIGHT });
	upgradeCard2 = sf::RectangleShape({ UPGRADE_CARD_WIDTH, UPGRADE_CARD_HEIGHT });
	upgradeCard3 = sf::RectangleShape({ UPGRADE_CARD_WIDTH, UPGRADE_CARD_HEIGHT });
	sf::RectangleShape* cards[] = { &upgradeCard1, &upgradeCard2, &upgradeCard3 };

	card1Title = sf::RectangleShape({ UPGRADE_CARD_WIDTH, 60 });
	card2Title = sf::RectangleShape({ UPGRADE_CARD_WIDTH, 60 });
	card3Title = sf::RectangleShape({ UPGRADE_CARD_WIDTH, 60 });
	sf::RectangleShape* titles[] = { &card1Title, &card2Title, &card3Title };

	float posY = windowHeight / 2;

	for (int i = 0; i < 3; i++)
	{
		float posX = 0 + (i + 1) * UPGRADE_CARD_GAP + i * UPGRADE_CARD_WIDTH + UPGRADE_CARD_WIDTH / 2.f;

		cards[i]->setOrigin(cards[i]->getGeometricCenter());
		cards[i]->setPosition({ posX, posY });
		cards[i]->setFillColor(sf::Color::Black);
		cards[i]->setOutlineThickness(OUTLINE_THICKNESS);

		titles[i]->setOrigin(titles[i]->getGeometricCenter());
		titles[i]->setPosition({ posX, posY - UPGRADE_CARD_HEIGHT / 2 + titles[i]->getGlobalBounds().size.y / 2});
		titles[i]->setFillColor(sf::Color::White);
		titles[i]->setOutlineThickness(OUTLINE_THICKNESS);
	}
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
		int cardIndex = GetClickedCardIndex(mouseWorldPos);
		if (cardIndex == -1)
		{
			ResetCardHoverEffect(upgradeCard1, card1Title, card1TitleText, card1DescriptionText);
			ResetCardHoverEffect(upgradeCard2, card2Title, card2TitleText, card2DescriptionText);
			ResetCardHoverEffect(upgradeCard3, card3Title, card3TitleText, card3DescriptionText);
			return;
		}

		if (upgradeCard1.getGlobalBounds().contains(mouseWorldPos))
		{
			OnCardHoverEffect(upgradeCard1, card1Title, card1TitleText, card1DescriptionText);

			ResetCardHoverEffect(upgradeCard2, card2Title, card2TitleText, card2DescriptionText);
			ResetCardHoverEffect(upgradeCard3, card3Title, card3TitleText, card3DescriptionText);
		}
		
		if (upgradeCard2.getGlobalBounds().contains(mouseWorldPos))
		{
			OnCardHoverEffect(upgradeCard2, card2Title, card2TitleText, card2DescriptionText);

			ResetCardHoverEffect(upgradeCard1, card1Title, card1TitleText, card1DescriptionText);
			ResetCardHoverEffect(upgradeCard3, card3Title, card3TitleText, card3DescriptionText);
		}
		
		if (upgradeCard3.getGlobalBounds().contains(mouseWorldPos))
		{
			OnCardHoverEffect(upgradeCard3, card3Title, card3TitleText, card3DescriptionText);

			ResetCardHoverEffect(upgradeCard1, card1Title, card1TitleText, card1DescriptionText);
			ResetCardHoverEffect(upgradeCard2, card2Title, card2TitleText, card2DescriptionText);
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
		gameWindow.draw(upgradeCard1);
		gameWindow.draw(card1Title);
		gameWindow.draw(*card1TitleText);
		gameWindow.draw(*card1DescriptionText);

		gameWindow.draw(upgradeCard2);
		gameWindow.draw(card2Title);
		gameWindow.draw(*card2TitleText);
		gameWindow.draw(*card2DescriptionText);

		gameWindow.draw(upgradeCard3);
		gameWindow.draw(card3Title);
		gameWindow.draw(*card3TitleText);
		gameWindow.draw(*card3DescriptionText);
	}
}

void UIManager::ShowRandomUpgrades(const std::vector<Upgrade>& upgrades)
{
	std::optional<sf::Text>* titleTexts[] = { &card1TitleText, &card2TitleText, &card3TitleText };
	std::optional<sf::Text>* descriptionTexts[] = { &card1DescriptionText, &card2DescriptionText, &card3DescriptionText };
	sf::RectangleShape* cards[] = { &upgradeCard1, &upgradeCard2, &upgradeCard3 };
	sf::RectangleShape* titles[] = { &card1Title, &card2Title, &card3Title };

	for (int i = 0; i < 3; i++)
	{
		sf::Color rarityColor = GetColorFromRarity(upgrades[i].rarity);
		cards[i]->setOutlineColor(rarityColor);

		// Set title texts
		titleTexts[i]->emplace(font, upgrades[i].title, 40);
		(*titleTexts[i])->setFillColor(sf::Color::Black);
		sf::FloatRect tBounds = (*titleTexts[i])->getLocalBounds();
		(*titleTexts[i])->setOrigin({ tBounds.size.x / 2, tBounds.size.y });
		(*titleTexts[i])->setPosition(titles[i]->getPosition());
		titles[i]->setOutlineColor(rarityColor);

		// Set description texts
		std::string wrapped = WrapText(upgrades[i].description, UPGRADE_CARD_WIDTH - 20.f, 20);
		descriptionTexts[i]->emplace(font, wrapped, 20);
		(*descriptionTexts[i])->setFillColor(sf::Color::White);
		sf::FloatRect dBounds = (*descriptionTexts[i])->getGlobalBounds();
		(*descriptionTexts[i])->setOrigin({ dBounds.size.x / 2, dBounds.size.y });
		(*descriptionTexts[i])->setPosition({ cards[i]->getPosition().x, cards[i]->getPosition().y });
	}
}

int UIManager::GetClickedCardIndex(const sf::Vector2f& mousePos) const
{
	if (upgradeCard1.getGlobalBounds().contains(mousePos)) return 0;
	if (upgradeCard2.getGlobalBounds().contains(mousePos)) return 1;
	if (upgradeCard3.getGlobalBounds().contains(mousePos)) return 2;

	return -1;
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
	button.setScale({ 1.05f, 1.05f });
	text->setFillColor(sf::Color::Black);
}

void UIManager::ResetHoverEffect(sf::RectangleShape& button, std::optional<sf::Text>& text)
{
	button.setFillColor(sf::Color::Black);
	text->setFillColor(sf::Color::White);
	button.setScale({ 1.0f, 1.0f });
}

void UIManager::OnCardHoverEffect(sf::RectangleShape& card, sf::RectangleShape& title,
	std::optional<sf::Text>& titleText, std::optional<sf::Text>& description)
{
	card.setFillColor(sf::Color::White);
	title.setFillColor(sf::Color::White);
	titleText->setFillColor(sf::Color::Black);
	description->setFillColor(sf::Color::Black);
}

void UIManager::ResetCardHoverEffect(sf::RectangleShape& card, sf::RectangleShape& title,
	std::optional<sf::Text>& titleText, std::optional<sf::Text>& description)
{
	card.setFillColor(sf::Color::Black);
	title.setFillColor(sf::Color::Black);
	titleText->setFillColor(sf::Color::White);
	description->setFillColor(sf::Color::White);
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
