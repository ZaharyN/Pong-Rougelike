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

enum class ColorState
{
	Red = 1,
	Yellow = 2,
	Green = 3,
	Turquoise = 4,
	Blue = 5,
	Purple = 6
};

struct PlayerControls
{
	sf::Keyboard::Scancode leftPrimary;
	sf::Keyboard::Scancode leftSecondary = sf::Keyboard::Scancode::Unknown;

	sf::Keyboard::Scancode rightPrimary;
	sf::Keyboard::Scancode rightSecondary = sf::Keyboard::Scancode::Unknown;
};