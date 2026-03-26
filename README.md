# Pong Rougelike:
The classic pong game created with SFML 3.0 with some rougelike elements. Players collect energy to unlock powerful upgrades, creating a dynamic and escalating gameplay experience.

## 🚀 Features

- **Roguelike Progression**: Collect 3 energy to trigger a Level Up. Choose from 3 random upgrades with varying rarities (**Common, Uncommon, Legendary**).
- **Advanced OOP Architecture**: Built using a Manager-based design pattern (GameManager, AudioManager, UIManager, UpgradeManager) for high scalability and clean code.
- **Smart AI**: An adaptive Enemy AI that tracks ball trajectory and utilizes "Dash" abilities strategically.

## 🛠 Tech Stack

- **Language**: C++20
- **Framework**: SFML 3.0 (Graphics, Windowing, Audio)
- **Tooling**: Visual Studio 2022, CMake

## 🎮 Controls

### vs AI (One Player)
- **Player 1**: `A` / `D` or `Left` / `Right` arrows to move.
- **Ability**: `Space` to Dash (once unlocked).

### vs Player (Two Player)
- **Player 1 (Bottom)**: `Left` / `Right` arrows to move.
- **Player 2 (Top)**: `A` / `D` to move.
- **Ability**: `Space` to Dash (once unlocked).

### General
- **UI**: Mouse Left-Click to navigate menus and select upgrades.
- **Exit**: `Escape` to quit.

## 🏗 Architecture Highlights

- **Manager Pattern**: Each game system is isolated. `GameManager` acts as the brain of the application. `AudioManager` handles a non-blocking sound pool; `UpgradeManager` handles weighted probability logic.
- **Polymorphism**: The `Paddle` base class allows the game to treat `Player` and `Enemy` identically in the update loop.
- **Data-Driven Upgrades**: Upgrades are stored as a "pool" of functional objects, making it trivial to add new game mechanics without modifying existing class logic.

## 🔧 How to Build (Source)

1. Ensure you have **SFML 3.0** installed.
2. Clone the repository.
3. Open the folder in Visual Studio or use CMake to generate project files.
4. Build in `Release` mode for best performance.

## 📦 How to Play (Release)

- I will add soon the release files