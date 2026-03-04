#include "GameManager.h"

GameManager::GameManager()
	: gameWindow(sf::VideoMode({ windowWidth, windowHeight }), "PONG GAME"),
	gameState(GameState::Menu)
{
	gameWindow.setFramerateLimit(60);

	audioManager = std::make_unique<AudioManager>();
	uiManager = std::make_unique<UIManager>(windowWidth);
}

void GameManager::Run()
{
	float deltaTime = clock.restart().asSeconds();

	while (gameWindow.isOpen())
	{
		ProcessEvents();
		Update(deltaTime);
		CheckCollisions();
		Render();
	}
}

void GameManager::ProcessEvents()
{
	while (const std::optional event = gameWindow.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			gameWindow.close();
		}
		else if (const sf::Event::KeyPressed* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed->scancode == sf::Keyboard::Scan::Escape)
			{
				gameWindow.close();
			}
		}

		if (const auto* mouseClick = event->getIf<sf::Event::MouseButtonPressed>())
		{
			if (mouseClick->button == sf::Mouse::Button::Left)
			{
				if (gameState == GameState::Playing) continue;

				sf::Vector2i pixelPosition = sf::Mouse::getPosition(gameWindow);
				sf::Vector2f mouseWorldPos = gameWindow.mapPixelToCoords(pixelPosition);

				if (gameState == GameState::Menu)
				{
					if (uiManager->IsStartClicked(mouseWorldPos))
					{
						gameState = GameState::ModeSelect;
						audioManager->PlaySound("select");
					}
					else if (uiManager->IsExitClicked(mouseWorldPos))
					{
						gameWindow.close();
					}
				}
				else if (gameState == GameState::ModeSelect)
				{
					if (uiManager->IsOnePlayerClicked(mouseWorldPos))
					{
						gameState = GameState::Playing;
						audioManager->PlaySound("select");
						audioManager->PlayBackgroundMusic();

						StartGame(GameMode::OnePlayer);
					}
					else if (uiManager->IsTwoPlayerClicked(mouseWorldPos))
					{
						gameState = GameState::Playing;
						audioManager->PlaySound("select");
						audioManager->PlayBackgroundMusic();

						StartGame(GameMode::TwoPlayer);
					}
				}
			}
		}
	}
}

void GameManager::StartGame(GameMode gameMode)
{
	
}

void GameManager::Update(float deltaT)
{
	if (gameState == GameState::Playing)
	{
		// Add checks
		player1->Update(deltaT);
		player2->Update(deltaT);
		ball->Update(deltaT);
	}
	else
	{
		uiManager->Update(gameState, gameWindow);
	}
}

void GameManager::CheckCollisions()
{

}

void GameManager::Render()
{
	gameWindow.clear();

	if (gameState == GameState::Playing)
	{

	}
	else
	{
		uiManager->Render(gameState, gameWindow);
	}
}

GameManager::~GameManager()
{

}