#pragma once
#include <string>
#include <SFML/Window/Keyboard.hpp>
#include <functional>
#include <Ball.h>
#include <Paddle.h>

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

enum class UpgradeRarity
{
	Common,
	Uncommon,
	Legendary,
};

struct Upgrade
{
	std::string title;
	std::string description;
	UpgradeRarity rarity;
	std::function<void(Paddle&, Paddle&, Ball&)> action;
};
