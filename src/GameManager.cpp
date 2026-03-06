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
	while (gameWindow.isOpen())
	{
		float deltaTime = clock.restart().asSeconds();

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
	ball = std::make_unique<Ball>(ballRadius, sf::Vector2f{ windowWidth / 2.f, windowHeight / 2.f },
		ballInitialSpeed, initialMinAngle, initialMaxAngle, sf::Color::Red);

	PlayerControls p1Controls;
	PlayerControls p2Controls;

	if (gameMode == GameMode::OnePlayer)
	{
		p1Controls =
		{
			sf::Keyboard::Scancode::Left, sf::Keyboard::Scancode::A,
			sf::Keyboard::Scancode::Right, sf::Keyboard::Scancode::D
		};

		player2 = std::make_unique<Enemy>(
			sf::Vector2f{ playerLength, playerHeight }, PaddleScreenPosition::Top,
			sf::Vector2f{ windowWidth / 2.f, 0 + playerHeight / 2.f },
			sf::Color::White,
			playerSpeed, *ball, windowWidth);
	}
	else
	{
		p1Controls =
		{
			sf::Keyboard::Scancode::Left, sf::Keyboard::Scancode::Unknown,
			sf::Keyboard::Scancode::Right, sf::Keyboard::Scancode::Unknown
		};
		p2Controls =
		{
			sf::Keyboard::Scancode::A, sf::Keyboard::Scancode::Unknown,
			sf::Keyboard::Scancode::D, sf::Keyboard::Scancode::Unknown
		};

		player2 = std::make_unique<Player>(
			sf::Vector2f{ playerLength, playerHeight }, PaddleScreenPosition::Top,
			sf::Vector2f{ windowWidth / 2.f, 0 + playerHeight / 2 },
			sf::Color::White,
			playerSpeed, windowWidth, p2Controls);
	}

	player1 = std::make_unique<Player>(
		sf::Vector2f{ playerLength, playerHeight }, PaddleScreenPosition::Bottom,
		sf::Vector2f{ windowWidth / 2.f, windowHeight - playerHeight / 2 },
		sf::Color::White,
		playerSpeed, windowWidth, p1Controls);
}

void GameManager::Update(float deltaT)
{
	if (gameState == GameState::Playing)
	{
		if (player1 && player2 && ball)
		{
			ball->Update(deltaT);
			player1->Update(deltaT);
			player2->Update(deltaT);
		}
	}
	else
	{
		uiManager->Update(gameState, gameWindow);
	}
}

void GameManager::CheckCollisions()
{
	if (gameState == GameState::Playing)
	{
		if (ball->GetBody().getPosition().x + ballRadius >= windowWidth
			|| ball->GetBody().getPosition().x - ballRadius <= 0)
		{
			ball->SwapHorizontalDirection();
			audioManager->PlaySound("hit");
		}

		if (ball->GetGlobalBounds().findIntersection(player1->GetGlobalBounds()))
		{
			ball->SetPosition({ ball->GetBody().getPosition().x, player1->GetBody().getPosition().y - (ballRadius + playerHeight / 2) });
			ball->ApplySpin(player1->GetXDirection());
			ball->IncreaseSpeed();

			audioManager->PlaySound("hit");
			audioManager->SetPitch(ball->GetCurrentSpeed() / ball->GetInitialSpeed());
		}
		else if (ball->GetGlobalBounds().findIntersection(player2->GetGlobalBounds()))
		{
			ball->SetPosition({ ball->GetBody().getPosition().x, player2->GetBody().getPosition().y + (ballRadius + playerHeight / 2) });
			ball->ApplySpin(player2->GetXDirection());
			ball->IncreaseSpeed();

			audioManager->PlaySound("hit");
			audioManager->SetPitch(ball->GetCurrentSpeed() / ball->GetInitialSpeed());
		}

		if (ball->GetBody().getPosition().y - ballRadius <= 0
			|| ball->GetBody().getPosition().y + ballRadius >= windowHeight)
		{
			player1->Reset();
			player2->Reset();
			ball->Reset();

			audioManager->StopBackgroundMusic();

			gameState = GameState::Menu;
		}
	}
}

void GameManager::Render()
{
	gameWindow.clear();

	if (gameState == GameState::Playing)
	{
		player1->Draw(gameWindow);
		player2->Draw(gameWindow);
		ball->Draw(gameWindow);
	}
	else
	{
		uiManager->Render(gameState, gameWindow);
	}

	gameWindow.display();
}
 
GameManager::~GameManager()
{

}