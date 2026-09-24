# PacMan

A RayLib-based recreation of the classic PacMan game for CSE-102, BUET 1-1.

## Developers
- Mahir Ahmed
- Raihan Biswas

---

## Core Features

### Maze & Grid System ✅ Done
- 31x28 grid-based maze stored in a 2D character array
- Classic PacMan-style walls, corridors, ghost house, tunnels, dots, and power dots
- Separate decision grid used by the ghost navigation routine
- Walls block PacMan and ghost movement
- Horizontal tunnel wrap-around at both maze edges
- Ghost-house gate is restricted for ghost movement

### Main Character Movement & Collision Management ✅ Done
- Smooth frame-time-based movement in four directions
- Arrow-key controls for left, right, up, and down
- Directional PacMan animation with three frames for each direction
- Idle sprite when PacMan is not moving
- Wall collision handling at grid positions
- Movement begins after a four-second round countdown

### Ghost AI & Movement ✅ Done
- **Blinky (Red Ghost)** ✅ Implemented
  - Uses PacMan's position as its chase target
  - Begins moving after the countdown
- **Pinky (Pink Ghost)** ✅ Implemented
  - Targets a position ahead of PacMan's current movement
  - Released after Blinky
- **Inky (Cyan Ghost)** ✅ Implemented
  - Calculates a target from PacMan's position and Blinky's position
  - Released after Pinky
- **Clyde (Orange Ghost)** ✅ Implemented
  - Chases PacMan when it is far from Blinky
  - Moves toward the lower-left target area when close to Blinky
- Shared `ghost_direction` function for decision-point navigation
- Ghost movement avoids walls and the ghost-house gate where required
- Ghosts wrap around the horizontal tunnel
- Shared `ghost_bounce` function reverses ghost speed and direction after ghost-to-ghost collisions
- Staggered ghost release during each round

### Dot Collection & Point Management ✅ Done
- Regular dots: 240 total, worth 10 points each
- Large power dots: 4 total, worth 50 points each
- Apple: worth 200 points
- Running score displayed during gameplay
- Regular dots disappear after collection and play the chomp sound
- Power dots disappear after collection and play the fruit/eat sound
- Apple appears at a fixed maze position after the regular-dot count reaches 150 or fewer
- Apple disappears after eight seconds if it is not collected

### Death, Game Over & Respawn 🟡 Partially Done
- 3-life system with heart icons in the HUD
- Collision detection between PacMan and all four ghosts
- Losing a life resets PacMan and every ghost to its starting position
- A new countdown starts after a collision
- Losing all lives resets the score, restores the three lives, and returns to the start menu
- Clearing all regular and power dots starts a new round
- ❌ No dedicated Game Over screen or final-score display
- ❌ No high-score persistence

### Invincibility/Power-Up Mode ❌ Remaining
- Power-up mechanic after PacMan eats a large power dot
- Ability for PacMan to eat ghosts during power-up
- Ghost vulnerability and ghost-house respawn behavior
- Power-up visual, audio, and timing effects

### Audio & Animations 🟡 Partially Done
- ✅ PacMan directional animation
- ✅ Idle PacMan sprite
- ✅ Pulsing large power-dot rendering
- ✅ Chomp sound effect for regular dots
- ✅ Fruit/eat sound effect for power dots and the apple
- ❌ Power-up mode audio and animations
- ❌ Death/game-over sound effects
- ❌ Ghost mode-switch effects

### Menu System 🟡 Partially Done
- ✅ Start menu with PacMan logo
- ✅ Play button
- ✅ Decorative HUD artwork
- ❌ Game mode selection
- ❌ Sprite/character customization
- ❌ Settings/audio management
- ❌ How-to-play or instructions screen

### Countdown & Round Management 🟡 Partially Done
- ✅ Four-second countdown before each round
- ✅ `3`, `2`, `1`, and `READY` display
- ✅ Staggered ghost release
- ✅ Round restart after all regular and power dots are collected
- ❌ Dedicated Game Over screen
- ❌ High-score tracking and display
- ❌ Persistent progress or level-selection system

---

## Advanced Features

### Individual Ghost Features 🟡 Partially Done
- ✅ Four ghost sprites: Blinky, Pinky, Inky, and Clyde
- ✅ Individual target calculations for all four ghosts
- ✅ Staggered release timing
- ✅ Ghost-to-ghost collision bounce behavior
- ✅ Horizontal tunnel movement
- 🟡 Target-based directional movement is implemented, but it is not a complete reproduction of the original arcade pathfinding system
- ❌ Power-mode vulnerability and ghost-eating behavior
- ❌ Eaten-ghost return to the ghost house

### Unconventional Game Modes ❌ Remaining
- **Mode 1**: Firing mechanic for PacMan and/or ghosts
- **Mode 2**: Wall-jumping ability for PacMan and/or ghosts
- **Mode 3**: Escape portals for PacMan
- **Mode 4**: Temporary speed, invisibility, or other special boosts
- **Mode 5**: Reverse hunter mode

---

## Project Statistics

| Component | Status | Notes |
|-----------|--------|-------|
| Core Gameplay | ✅ 85% | Playable maze, movement, scoring, lives, and four ghosts are implemented |
| Ghost System | ✅ 85% | Four ghosts have distinct targeting behavior and collision bounce handling |
| Feature Completeness | 🟡 65% | Power mode, game-over UI, persistence, and advanced modes remain |
| Overall Progress | 🟡 70% | Strong playable foundation with several planned gameplay and polish features remaining |

---

## How to Build & Run

### Requirements
- RayLib library
- C compiler such as GCC, Clang, or MSVC
- Desktop environment capable of opening a RayLib window

### Build
```bash
gcc main.c -o main -lraylib -lm
```

Depending on the platform and RayLib installation, additional linker libraries may be required.

### Run
```bash
./main
```

On Windows, the repository includes `main.exe`. Run the executable from the repository root so that the relative `assets/` paths resolve correctly.

---

## Game Controls

- **Arrow Keys**: Move PacMan
- **Left Mouse Click**: Start the game by clicking the play button
- **ESC**: Close the game window

---

## File Structure

```
PacMan/
├── main.c                         # Maze, game loop, movement, scoring, ghost AI, and rendering
├── main.exe                       # Prebuilt Windows executable
├── raylib_template.code-workspace # VS Code workspace configuration
├── record.txt                     # Repository file; score persistence is not currently implemented in main.c
├── assets/                        # Game resources
│   ├── bg.png                     # Maze background
│   ├── idle.png                   # PacMan idle sprite
│   ├── pacman-logo.png            # Menu logo
│   ├── play-button.png            # Menu play button
│   ├── pacman-left/               # Left-facing PacMan animation frames
│   ├── pacman-right/              # Right-facing PacMan animation frames
│   ├── pacman-up/                 # Up-facing PacMan animation frames
│   ├── pacman-down/               # Down-facing PacMan animation frames
│   ├── ghosts/                    # Blinky, Pinky, Inky, and Clyde sprites
│   ├── other/                     # Apple, heart, and HUD textures
│   └── audio/                     # Chomp and fruit/eat sound effects
└── README.md                      # This file
```

---

## Implementation Notes

- The gameplay is currently implemented in a single `main.c` file.
- RayLib and Raymath provide the window, texture, audio, vector, animation, and collision functionality.
- The maze uses `#` for walls, `.` for regular dots, `o` for large power dots, `-` for the ghost-house gate, and runtime markers for collected or temporary items.
- Ghost decision points are marked with `d` in the separate `decision` grid.
- Ghost movement uses directional target comparisons at decision points rather than full shortest-path search.
- Ghost and PacMan movement uses a base speed of 250 and frame-time scaling.
- The game loads all assets using relative paths, so the working directory must contain the `assets` directory.

---

## Next Steps

1. Implement power-up mode and vulnerable ghost behavior
2. Add ghost-eating and ghost-house respawn logic
3. Create a dedicated Game Over screen with the final score
4. Add high-score persistence using `record.txt`
5. Add power-up, death, and game-over audio effects
6. Implement an instructions screen and settings menu
7. Refactor the single source file into multiple modules
8. Implement unconventional game modes
