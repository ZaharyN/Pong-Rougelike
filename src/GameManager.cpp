#include "GameManager.h"

GameManager::GameManager()
	: gameWindow(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "PONG GAME"),
	gameState(GameState::Menu)
{
	gameWindow.setFramerateLimit(60);

	audioManager = std::make_unique<AudioManager>();
	uiManager = std::make_unique<UIManager>(WINDOW_WIDTH, WINDOW_HEIGHT);
	collectibleManager = std::make_unique<CollectibleManager>(WINDOW_WIDTH, WINDOW_HEIGHT);
	upgradeManager = std::make_unique<UpgradeManager>();

	currentUpgradeOptions.reserve(3);
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
					ClickTarget clickedTarget = uiManager->GetClickedTarget(mouseWorldPos, gameState);

					if (clickedTarget == ClickTarget::StartButton)
					{
						gameState = GameState::ModeSelect;
						audioManager->PlaySound("select");
					}
					else if (clickedTarget == ClickTarget::ExitButton)
					{
						gameWindow.close();
					}
				}
				else if (gameState == GameState::ModeSelect)
				{
					ClickTarget clickedTarget = uiManager->GetClickedTarget(mouseWorldPos, gameState);

					if (clickedTarget == ClickTarget::OnePlayerOption)
					{
						gameState = GameState::Playing;
						audioManager->PlaySound("select");
						audioManager->PlayBackgroundMusic();

						StartGame(GameMode::OnePlayer);
					}
					else if (clickedTarget == ClickTarget::TwoPlayerOption)
					{
						gameState = GameState::Playing;
						audioManager->PlaySound("select");
						audioManager->PlayBackgroundMusic();

						StartGame(GameMode::TwoPlayer);
					}
				}
				else if (gameState == GameState::UpgradeSelect)
				{
					int pickedCard = uiManager->GetClickedCardIndex(mouseWorldPos);

					if (pickedCard != -1)
					{
						currentUpgradeOptions[pickedCard].action(*upgradeRecipient, *upgradeVictim, *ball);
						gameState = GameState::Playing;
						audioManager->PlaySound("select");
					}
				}
			}
		}
	}
}

void GameManager::StartGame(GameMode gameMode)
{
	ball = std::make_unique<Ball>(BALL_RADIUS, sf::Vector2f{ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f },
		BALL_INITIAL_SPEED, INITIAL_MIN_ANGLE, INITIAL_MAX_ANGLE, sf::Color::Red);

	PlayerControls p1Controls;
	PlayerControls p2Controls;

	if (gameMode == GameMode::OnePlayer)
	{
		p1Controls =
		{
			sf::Keyboard::Scancode::Left, sf::Keyboard::Scancode::A,
			sf::Keyboard::Scancode::Right, sf::Keyboard::Scancode::D,
			sf::Keyboard::Scancode::Up, sf::Keyboard::Scancode::W,
			sf::Keyboard::Scancode::Down, sf::Keyboard::Scancode::S
		};

		player2 = std::make_unique<Enemy>(
			sf::Vector2f{ PLAYER_WIDTH, PLAYER_HEIGHT }, PaddleScreenPosition::Top,
			sf::Vector2f{ WINDOW_WIDTH / 2.f, PLAYER_HEIGHT / 2.f },
			sf::Color::Green,
			PLAYER_INITIAL_SPEED, *ball, WINDOW_WIDTH, WINDOW_HEIGHT, 100);
	}
	else
	{
		p1Controls =
		{
			sf::Keyboard::Scancode::Left, sf::Keyboard::Scancode::Unknown,
			sf::Keyboard::Scancode::Right, sf::Keyboard::Scancode::Unknown,
			sf::Keyboard::Scancode::Up, sf::Keyboard::Scancode::Unknown,
			sf::Keyboard::Scancode::Down, sf::Keyboard::Scancode::Unknown
		};
		p2Controls =
		{
			sf::Keyboard::Scancode::A, sf::Keyboard::Scancode::Unknown,
			sf::Keyboard::Scancode::D, sf::Keyboard::Scancode::Unknown,
			sf::Keyboard::Scancode::W, sf::Keyboard::Scancode::Unknown,
			sf::Keyboard::Scancode::S, sf::Keyboard::Scancode::Unknown
		};

		player2 = std::make_unique<Player>(
			sf::Vector2f{ PLAYER_WIDTH, PLAYER_HEIGHT }, PaddleScreenPosition::Top,
			sf::Vector2f{ WINDOW_WIDTH / 2.f, PLAYER_HEIGHT / 2 },
			sf::Color::Green,
			PLAYER_INITIAL_SPEED, WINDOW_WIDTH, WINDOW_HEIGHT, 100, p2Controls);
	}

	player1 = std::make_unique<Player>(
		sf::Vector2f{ PLAYER_WIDTH, PLAYER_HEIGHT }, PaddleScreenPosition::Bottom,
		sf::Vector2f{ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - PLAYER_HEIGHT / 2 },
		sf::Color::Green,
		PLAYER_INITIAL_SPEED, WINDOW_WIDTH, WINDOW_HEIGHT, 100, p1Controls);
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
			collectibleManager->Update(deltaT, player1.get(), player2.get());

			if (player1->HasForesight())
				player1->TrimForesight(ball->GetBody().getPosition().y, ball->GetVerticalDirection());
			if (player2->HasForesight())
				player2->TrimForesight(ball->GetBody().getPosition().y, ball->GetVerticalDirection());

			for (const auto& buddy : player1->GetBuddies())
				buddy->Update(deltaT);

			for (const auto& buddy : player2->GetBuddies())
				buddy->Update(deltaT);
		}
	}
	else
	{
		uiManager->Update(gameState, gameWindow);
	}
}

void GameManager::CheckCollisions()
{
	if (gameState != GameState::Playing) return;

	CheckWallCollisions();
	CheckPaddleCollisions();
	CheckBuddyCollisions();
	CheckObstacleCollisions();
	CheckDeadZone();
	CheckCollectibleCollisions();
}

void GameManager::CheckWallCollisions()
{
	auto handleWallCollision = [&](float correctedX)
		{
			ball->SetPosition({ correctedX, ball->GetBody().getPosition().y });
			ball->SwapHorizontalDirection();
			ball->ResetCurvature();
			audioManager->PlaySound("hit");
		};

	if (ball->GetBody().getPosition().x + BALL_RADIUS >= WINDOW_WIDTH)
		handleWallCollision(WINDOW_WIDTH - BALL_RADIUS);
	else if (ball->GetBody().getPosition().x - BALL_RADIUS <= 0)
		handleWallCollision(BALL_RADIUS);
}

void GameManager::CheckPaddleCollisions()
{
	auto handlePaddleCollissions = [&](Paddle* hitter, Paddle* receiver, float correctedY)
		{
			ball->SetPosition({ ball->GetBody().getPosition().x, correctedY });
			ball->ApplySpin(player1->GetXDirection(), hitter->GetSpinMultiplier(), hitter->GetCurvaturePower());
			ball->IncreaseSpeed();

			if (receiver->HasForesight())
				receiver->ComputeForesight(*ball);

			audioManager->PlaySound("hit");
			audioManager->SetPitch(ball->GetCurrentSpeed() / ball->GetInitialSpeed());

			hitter->UpdateEnergy(-1);
		};

	if (ball->GetVerticalDirection() > 0 && ball->GetGlobalBounds().findIntersection(player1->GetGlobalBounds()))
	{
		float correctedY = player1->GetBody().getPosition().y - (BALL_RADIUS + PLAYER_HEIGHT / 2.f) - BALL_OVERLAP_CORRECTION;
		handlePaddleCollissions(player1.get(), player2.get(), correctedY);
	}
	else if (ball->GetVerticalDirection() < 0 && ball->GetGlobalBounds().findIntersection(player2->GetGlobalBounds()))
	{
		float correctedY = player2->GetBody().getPosition().y + (BALL_RADIUS + PLAYER_HEIGHT / 2.f) + BALL_OVERLAP_CORRECTION;
		handlePaddleCollissions(player2.get(), player1.get(), correctedY);
	}
}

void GameManager::CheckBuddyCollisions()
{
	auto handleBuddyCollisions = [&](Paddle* buddyOwner, Paddle* receiver, float directionY)
		{
			for (const auto& buddy : buddyOwner->GetBuddies())
			{
				if (buddy->GetGlobalBounds().findIntersection(ball->GetGlobalBounds()))
				{
					float correctedY = buddy->GetBody().getPosition().y 
						+ directionY * (BALL_RADIUS + PLAYER_HEIGHT / 2.f + BALL_OVERLAP_CORRECTION);
					ball->SetPosition({ ball->GetBody().getPosition().x, correctedY });
					ball->SwapVerticalDirection();
					ball->IncreaseSpeed();

					if (receiver->HasForesight())
						receiver->ComputeForesight(*ball);

					audioManager->PlaySound("hit");
					break;
				}
			}
		};

	if (ball->GetVerticalDirection() > 0)
		handleBuddyCollisions(player1.get(), player2.get(), -1.f);
	if (ball->GetVerticalDirection() < 0)
		handleBuddyCollisions(player2.get(), player1.get(), 1.f);
}

void GameManager::CheckObstacleCollisions()
{
	auto handleObstacleCollisions = [&](Paddle* owner)
		{
			for (const auto& obs : owner->GetObstacles())
			{
				if (ball->GetGlobalBounds().findIntersection(obs.getGlobalBounds()))
				{
					ball->SwapVerticalDirection();
					audioManager->PlaySound("hit");
					break;
				}
			}
		};

	if (ball->GetVerticalDirection() < 0)
		handleObstacleCollisions(player2.get());
	if (ball->GetVerticalDirection() > 0)
		handleObstacleCollisions(player1.get());
}

void GameManager::CheckDeadZone()
{
	if (ball->GetBody().getPosition().y - BALL_RADIUS <= 0
		|| ball->GetBody().getPosition().y + BALL_RADIUS >= WINDOW_HEIGHT)
	{
		player1->Reset();
		player2->Reset();
		ball->Reset();
		collectibleManager->Reset();
		audioManager->StopBackgroundMusic();

		gameState = GameState::Menu;
		return;
	}
}

void GameManager::CheckCollectibleCollisions()
{
	collectibleManager->CheckCollisions(player1.get(), player2.get(), *audioManager);

	auto handleUpgradeSelection = [&](Paddle* recipient, Paddle* victim, std::string_view name)
		{
			recipient->ResetCollectedEnergy();
			upgradeRecipient = recipient;
			upgradeVictim = victim;
			currentUpgradeOptions = upgradeManager->ChooseThreeRandomUpgrades(recipient->GetOwnedUniqueUpgrades());
			uiManager->ShowRandomUpgrades(currentUpgradeOptions, name);
			gameState = GameState::UpgradeSelect;
		};

	if (player1->GetCollectedEnergy() == COLLECTIBLE_COUNT_FOR_UPGRADE)
		handleUpgradeSelection(player1.get(), player2.get(), "PLAYER_1");
	else if (player2->GetCollectedEnergy() == COLLECTIBLE_COUNT_FOR_UPGRADE)
		handleUpgradeSelection(player2.get(), player1.get(), "PLAYER_2");
}

void GameManager::Render()
{
	gameWindow.clear();

	if (gameState == GameState::Playing)
	{
		player1->Draw(gameWindow);
		player2->Draw(gameWindow);

		if (player1->HasForesight())
			player1->DrawForesight(gameWindow);
		if (player2->HasForesight())
			player2->DrawForesight(gameWindow);

		ball->Draw(gameWindow);

		for (const auto& obs : player1->GetObstacles())
			gameWindow.draw(obs);

		for (const auto& obs : player2->GetObstacles())
			gameWindow.draw(obs);

		for (const auto& buddy : player1->GetBuddies())
			gameWindow.draw(buddy->GetBody());

		for (const auto& buddy : player2->GetBuddies())
			gameWindow.draw(buddy->GetBody());

		collectibleManager->Draw(gameWindow);
	}
	else
	{
		uiManager->Draw(gameState, gameWindow);
	}

	gameWindow.display();
}