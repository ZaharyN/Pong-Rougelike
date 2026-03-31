# Pong Roguelike:
The classic pong game created with SFML 3.0 with some roguelike elements. Players collect energy to unlock powerful upgrades, creating a dynamic and escalating gameplay experience.

## 🚀 Features

- **Roguelike Progression**: Collect 3 energy to trigger a Level Up. Choose from 3 random upgrades with varying rarities (**Common, Uncommon, Legendary**).
- **Advanced OOP Architecture**: Built using a Manager-based design pattern (GameManager, AudioManager, UIManager, UpgradeManager) for high scalability and clean code.
- **Dual Game Modes**: Features a challenging **Single-player mode** against a reactive AI and a **Local PvP mode** for competitive play on a single machine.
- **Smart AI**: An adaptive Enemy AI that tracks ball trajectory and utilizes "Dash" abilities strategically.
- **Win Condition**: Matches are fast-paced and competitive—the first player to reach **11 points** is declared the winner.

## 🛠 Tech Stack

- **Language**: C++20
- **Framework**: SFML 3.0 (Graphics, Windowing, Audio)
- **Tooling**: Visual Studio 2022, CMake

## 🕹 How to Play

1. **Select Mode**: Choose between vs AI or vs Player in the main menu.
2. **Defend your Field**: Prevent the ball from entering your "Dead Zone" (top or bottom of the screen). 
3. **Collect Energy**: Yellow energy spawns randomly on your side of the field. Move your paddle to collect them.
4. **Level Up**: Every **3 energy points** collected allows you to choose an upgrade. These can affect speed, size, spin, or sabotage your opponent.
5. **Win the Match**: Anticipate the ball's path and utilize your upgrades to be the first to reach **11 points**.

## 🎮 Controls

### vs AI (One Player)
- **Movement**: `A` / `D` **or** `Left` / `Right` arrows to move.
- **Ability**: `Space` to **Dash** (once unlocked).

### vs Player (Two Player)
*   **Player 1 (Bottom)**: 
    *   **Move**: `Left` / `Right` arrows.
    *   **Ability**: `Space` to **Dash** (once unlocked).
*   **Player 2 (Top)**: 
    *   **Move**: `A` / `D` keys.
    *   **Ability**: `Left Shift` to **Dash** (once unlocked).

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

1. Go to the **Releases** section of this repository.
2. Download the **PongRoguelike_v1.0.0.zip** file.
3. Extract the folder to your computer.
4. Run **PongGame.exe**.