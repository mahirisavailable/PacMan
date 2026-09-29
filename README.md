# PacMan: Wormhole Edition

A custom RayLib-based arcade game inspired by classic PacMan, built as a BUET CSE-102 project. This version goes beyond a simple clone by adding original gameplay twists, themed visuals, and a more polished game menu.

## Project Overview

This project recreates the core PacMan experience with a maze, collectible pellets, ghost AI, score system, lives, and power-up mechanics, while also introducing unique custom features such as:

- Wormhole mode for alternate maze traversal
- Tom & Jerry themed character and ghost styling
- Animated menus and game settings
- High-score persistence and sound effects

## Key Features

### Classic PacMan Gameplay
- 31x28 maze-based level layout
- Dot collection and power-dot mechanics
- PacMan movement with smooth directional control
- Collision detection with walls and maze boundaries
- Ghost chase logic with staggered release timings
- Lives system and round management

### Wormhole Mode (Original Feature)
- Special warp-style mechanic between opposite maze corners
- Enables alternate traversal paths during gameplay
- Adds a distinct and playful twist to standard PacMan movement
- Makes the game feel more dynamic and custom rather than a direct copy

### Tom & Jerry Theme (Original Feature)
- Custom visual theme inspired by Tom & Jerry
- Ghosts are re-skinned in a themed style
- PacMan can switch between classic and themed appearance
- Brings a unique identity to the project and differentiates it from ordinary arcade clones

### Advanced Game Systems
- Four unique ghost behaviors: Blinky, Pinky, Inky, and Clyde
- Invincibility mode after eating a power pellet
- Ghosts slow down and reverse targeting during power mode
- PacMan can eat ghosts while invincible for bonus points
- Apple bonus fruit appears after enough dots are cleared
- High score is saved to `record.txt`

### User Experience
- Start menu with mode and theme selection
- Sound and music toggles
- How-to-play and credits screens
- Responsive in-game HUD with score, lives, and high score
- Animated sprite effects and smooth arcade presentation

## Gameplay Controls

- Arrow Keys: Move PacMan
- Left Mouse Click: Start game / select menu options
- ESC: Exit the game

## Installation

Install all the files in a folder & run `main.exe`

### Run

```bash
./main
```

## Project Structure

```text
PacMan/
├── main.c                     # Main game logic
├── main.exe                   # Windows executable
├── record.txt                 # High score storage
├── README.md                  # Project documentation
├── assets/                    # Sprites, textures, sounds, menu assets
    ├── bg.png
    ├── idle.png
    ├── pacman-logo.png
    ├── play-button.png
    ├── ghosts/
    ├── other/
    ├── theme1/
    ├── buttons/
    └── audio/
```

## Credits

### Developers
- Mahir Ahmed
- Raihan Biswas

### Educational Context
- Course: CSE-102, BUET 1-1
- Project Type: Academic game development project

### Tech Stack
- Languages: `C`
- Libraries: [`RayLib`](https://www.raylib.com/)

### Inspiration
- Classic PacMan arcade gameplay
- Original custom modifications by the project developers

## Notes

This project is not just a direct PacMan replica. It includes custom features such as the Wormhole mode and a Tom & Jerry themed visual set, giving it a distinct identity while preserving the classic arcade feel.

## Final Note

The game is designed to be playable, polished, and educational while still retaining enough originality to stand apart from a basic clone. The combination of classic gameplay, custom theme, and wormhole mechanics makes it a unique implementation of PacMan for this project.
