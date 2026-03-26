#include "GameManager.h"

GameManager::GameManager()
	: gameWindow(sf::VideoMode({ windowWidth, windowHeight }), "PONG GAME"),
	gameState(GameState::Menu)
{
	gameWindow.setFramerateLimit(60);

	audioManager = std::make_unique<AudioManager>();
	uiManager = std::make_unique<UIManager>(windowWidth, windowHeight);
	collectibleManager = std::make_unique<CollectibleManager>(windowWidth, windowHeight);
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
					if (uiManager->GetClickedTarget(mouseWorldPos, gameState) == ClickTarget::StartButton)
					{
						gameState = GameState::ModeSelect;
						audioManager->PlaySound("select");
					}
					else if (uiManager->GetClickedTarget(mouseWorldPos, gameState) == ClickTarget::ExitButton)
					{
						gameWindow.close();
					}
				}
				else if (gameState == GameState::ModeSelect)
				{
					if (uiManager->GetClickedTarget(mouseWorldPos, gameState) == ClickTarget::OnePlayerOption)
					{
						gameState = GameState::Playing;
						audioManager->PlaySound("select");
						audioManager->PlayBackgroundMusic();

						StartGame(GameMode::OnePlayer);
					}
					else if (uiManager->GetClickedTarget(mouseWorldPos, gameState) == ClickTarget::TwoPlayerOption)
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
	ball = std::make_unique<Ball>(ballRadius, sf::Vector2f{ windowWidth / 2.f, windowHeight / 2.f },
		ballInitialSpeed, initialMinAngle, initialMaxAngle, sf::Color::Red);

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
			sf::Vector2f{ playerLength, playerHeight }, PaddleScreenPosition::Top,
			sf::Vector2f{ windowWidth / 2.f, playerHeight / 2.f },
			sf::Color::Green,
			playerSpeed, *ball, windowWidth, windowHeight, 100);
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
			sf::Vector2f{ playerLength, playerHeight }, PaddleScreenPosition::Top,
			sf::Vector2f{ windowWidth / 2.f, playerHeight / 2 },
			sf::Color::Green,
			playerSpeed, windowWidth, windowHeight, 100, p2Controls);
	}

	player1 = std::make_unique<Player>(
		sf::Vector2f{ playerLength, playerHeight }, PaddleScreenPosition::Bottom,
		sf::Vector2f{ windowWidth / 2.f, windowHeight - playerHeight / 2 },
		sf::Color::Green,
		playerSpeed, windowWidth, windowHeight, 100, p1Controls);
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
	if (gameState == GameState::Playing)
	{
		// 1.Hits wall
		if (ball->GetBody().getPosition().x + ballRadius >= windowWidth)
		{
			ball->SetPosition({ windowWidth - ballRadius, ball->GetBody().getPosition().y });
			ball->SwapHorizontalDirection();
			ball->ResetCurvature();
			audioManager->PlaySound("hit");
		}
		else if (ball->GetBody().getPosition().x - ballRadius <= 0)
		{
			ball->SetPosition({ ballRadius, ball->GetBody().getPosition().y });
			ball->SwapHorizontalDirection();
			ball->ResetCurvature();
			audioManager->PlaySound("hit");
		}

		// 2.Ricochets from player
		if (ball->GetVerticalDirection() > 0 && ball->GetGlobalBounds().findIntersection(player1->GetGlobalBounds()))
		{
			ball->SetPosition({ ball->GetBody().getPosition().x, player1->GetBody().getPosition().y - (ballRadius + playerHeight / 2.f) - 1 });
			ball->ApplySpin(player1->GetXDirection(), player1->GetSpinMultiplier(), player1->GetCurvaturePower());
			ball->IncreaseSpeed();

			if (player2->HasForesight())
				player2->ComputeForesight(*ball, windowWidth, windowHeight);

			audioManager->PlaySound("hit");
			audioManager->SetPitch(ball->GetCurrentSpeed() / ball->GetInitialSpeed());

			player1->UpdateEnergy(-1);
		}
		else if (ball->GetVerticalDirection() < 0 && ball->GetGlobalBounds().findIntersection(player2->GetGlobalBounds()))
		{
			ball->SetPosition({ ball->GetBody().getPosition().x, player2->GetBody().getPosition().y + (ballRadius + playerHeight / 2.f) + 1 });
			ball->ApplySpin(player2->GetXDirection(), player2->GetSpinMultiplier(), player2->GetCurvaturePower());
			ball->IncreaseSpeed();

			if (player1->HasForesight())
				player1->ComputeForesight(*ball, windowWidth, windowHeight);

			audioManager->PlaySound("hit");
			audioManager->SetPitch(ball->GetCurrentSpeed() / ball->GetInitialSpeed());

			player2->UpdateEnergy(-1);
		}

		// 3. Check buddies collisions
		if (ball->GetVerticalDirection() > 0)
		{
			for (const auto& buddy : player1->GetBuddies())
			{
				if (buddy->GetGlobalBounds().findIntersection(ball->GetGlobalBounds()))
				{
					ball->SetPosition({ ball->GetBody().getPosition().x, buddy->GetBody().getPosition().y - (ballRadius + playerHeight / 2.f) - 1 });
					ball->SwapVerticalDirection();
					ball->IncreaseSpeed();

					if (player2->HasForesight())
						player2->ComputeForesight(*ball, windowWidth, windowHeight);

					audioManager->PlaySound("hit");
					break;
				}
			}
		}
		if (ball->GetVerticalDirection() < 0)
		{
			for (const auto& buddy : player2->GetBuddies())
			{
				if (buddy->GetGlobalBounds().findIntersection(ball->GetGlobalBounds()))
				{
					ball->SetPosition({ ball->GetBody().getPosition().x, buddy->GetBody().getPosition().y + (ballRadius + playerHeight / 2.f) + 1 });
					ball->SwapVerticalDirection();
					ball->IncreaseSpeed();

					if (player1->HasForesight())
						player1->ComputeForesight(*ball, windowWidth, windowHeight);

					audioManager->PlaySound("hit");
					break;
				}
			}
		}

		// 4. Check obstacle collisions
		// Check if ball is towards top player
		if (ball->GetVerticalDirection() < 0)
		{
			for (const auto& obs : player2->GetObstacles())
			{
				if (ball->GetGlobalBounds().findIntersection(obs.getGlobalBounds()))
				{
					ball->SwapVerticalDirection();
					audioManager->PlaySound("hit");
					break;
				}
			}
		}
		// Check if ball is towards bottom player
		if (ball->GetVerticalDirection() > 0)
		{
			for (const auto& obs : player1->GetObstacles())
			{
				if (ball->GetGlobalBounds().findIntersection(obs.getGlobalBounds()))
				{
					ball->SwapVerticalDirection();
					audioManager->PlaySound("hit");
					break;
				}
			}
		}

		// 5.Hits dead zone
		if (ball->GetBody().getPosition().y - ballRadius <= 0
			|| ball->GetBody().getPosition().y + ballRadius >= windowHeight)
		{
			player1->Reset();
			player2->Reset();
			ball->Reset();
			collectibleManager->Reset();
			audioManager->StopBackgroundMusic();

			gameState = GameState::Menu;
			return;
		}

		// 6. Check collectibles pickup
		collectibleManager->CheckCollisions(player1.get(), player2.get(), *audioManager);

		if (player1->GetCollectedEnergy() == collectibleCountForUpgrade)
		{
			player1->ResetCollectedEnergy();
			upgradeRecipient = player1.get();
			upgradeVictim = player2.get();

			currentUpgradeOptions = upgradeManager->ChooseThreeRandomUpgrades(player1->GetOwnedUniqueUpgrades());
			uiManager->ShowRandomUpgrades(currentUpgradeOptions);

			gameState = GameState::UpgradeSelect;
		}
		else if (player2->GetCollectedEnergy() == collectibleCountForUpgrade)
		{
			player2->ResetCollectedEnergy();
			upgradeRecipient = player2.get();
			upgradeVictim = player1.get();

			currentUpgradeOptions = upgradeManager->ChooseThreeRandomUpgrades(player2->GetOwnedUniqueUpgrades());
			uiManager->ShowRandomUpgrades(currentUpgradeOptions);

			gameState = GameState::UpgradeSelect;
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