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
	GameOver
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
	None,
	StartButton,
	ExitButton,
	OnePlayerOption,
	TwoPlayerOption,
	UpgradeCard1,
	UpgradeCard2,
	UpgradeCard3,
};

enum class UpgradeType
{
	None,
	Sonic,
	Snail,
	Goliath,
	Midget,
	MoreSpin,
	LessSpin,
	Near,
	Far,
	Dash,
	Obstacles,
	Sping,
	Foresight,
	DumbBuddy,
	Crank,
	Extraganza
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

	sf::Keyboard::Scancode upPrimary = sf::Keyboard::Scancode::Unknown;
	sf::Keyboard::Scancode upSecondary = sf::Keyboard::Scancode::Unknown;

	sf::Keyboard::Scancode downPrimary = sf::Keyboard::Scancode::Unknown;
	sf::Keyboard::Scancode downSecondary = sf::Keyboard::Scancode::Unknown;
};

struct Upgrade
{
	UpgradeType type;
	std::string title;
	std::string description;
	UpgradeRarity rarity;
	bool isUnique;
	std::function<void(Paddle&, Paddle&, Ball&)> action;
};