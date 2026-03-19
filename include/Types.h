#pragma once
#include <string>
#include <SFML/Window/Keyboard.hpp>
#include <functional>

class Ball;
class Paddle;

enum class GameState
{
	Menu,
	ModeSelect,
	Playing,
	UpgradeSelect,
};

enum class GameMode
{
	OnePlayer,
	TwoPlayer,
};

enum class PaddleScreenPosition
{
	Top,
	Bottom,
};

enum class ColorState
{
	Red,
	Yellow,
	Green,
	Turquoise,
	Blue,
	Purple,
};

enum class ClickTarget
{
	StartButton,
	ExitButton,
	OnePlayerOption,
	TwoPlayerOption,
	UpgradeCard1,
	UpgradeCard2,
	UpgradeCard3,
};

enum class UpgradeRarity
{
	Common,
	Uncommon,
	Legendary,
};

struct PlayerControls
{
	sf::Keyboard::Scancode leftPrimary = sf::Keyboard::Scancode::Unknown;
	sf::Keyboard::Scancode leftSecondary = sf::Keyboard::Scancode::Unknown;

	sf::Keyboard::Scancode rightPrimary = sf::Keyboard::Scancode::Unknown;
	sf::Keyboard::Scancode rightSecondary = sf::Keyboard::Scancode::Unknown;
};

struct Upgrade
{
	std::string title;
	std::string description;
	UpgradeRarity rarity;
	std::function<void(Paddle&, Paddle&, Ball&)> action;
};
