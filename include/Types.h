#pragma once

enum class GameState
{
	Menu,
	ModeSelect,
	Playing,
};

enum class GameMode
{
	OnePlayer,
	TwoPlayer,
};

enum class PaddleScreenPosition
{
	Top = 1,
	Bottom = 2,
};

struct PlayerControls
{
	sf::Keyboard::Scancode leftPrimary;
	sf::Keyboard::Scancode leftSecondary = sf::Keyboard::Scancode::Unknown;

	sf::Keyboard::Scancode rightPrimary;
	sf::Keyboard::Scancode rightSecondary = sf::Keyboard::Scancode::Unknown;
};