# PacMan

A RayLib-based recreation of the classic PacMan game for CSE-102, BUET 1-1.

## Developers
- Mahir Ahmed
- Raihan Biswas

---

## Core Features

### Maze & Grid System ✅ Done
- 31x28 grid-based maze layout using a 2D character array
- Classic PacMan maze design with walls, corridors, ghost house, and tunnel passages
- Separate decision grid for ghost navigation
- Walls block both player and ghost movement
- Horizontal tunnel wrap-around at both maze edges

### Main Character Movement & Collision Management ✅ Done
- Smooth directional movement using frame-time-based velocity
- Four-direction movement: up, down, left, and right
- Directional sprite animations with three frames per direction
- Idle PacMan sprite when the player is stationary
- Collision detection with maze walls
- Direction changes are handled at grid intersections
- PacMan cannot pass through walls in the next cell while moving

### Ghost AI & Movement ✅ Done
- **Blinky (Red Ghost)** ✅ Implemented
  - Targets and chases PacMan during its chase phase
  - Starts moving after the 4-second countdown
- **Pinky (Pink Ghost)** ✅ Implemented
  - Targets a position ahead of PacMan based on its current movement
  - Starts moving after Blinky
- **Inky (Cyan Ghost)** ✅ Implemented
  - Calculates its target using PacMan's position and Blinky's position
  - Starts moving after Pinky
- **Clyde (Orange Ghost)** ✅ Implemented
  - Chases PacMan when far from Blinky
  - Retreats toward the lower-left area when close to Blinky
- All ghosts use the shared `ghost_direction` function for maze navigation
- Ghosts avoid walls and the ghost-house gate where appropriate
- Ghosts wrap around the horizontal tunnel
- Ghost collisions are handled through the reusable `ghost_bounce` function
- Ghost release is staggered during each round
- During invincible mode, ghosts reverse target direction and move slower

### Dot Collection & Point Management ✅ Done
- Regular dots (240 total): +10 points each
- Large power dots (4 total): +50 points each
- Special fruit (apple): +200 points when collected
- Running score display
- Apple appears after enough regular dots have been collected
- Apple remains available for eight seconds before disappearing
- Chomp sound effect for regular dots
- Fruit/eat sound effect for power dots and the apple

### Death, Game Over & Respawn ✅ Done
- 3-life system with heart display
- Collision detection between PacMan and all four ghosts
- One life is removed after a ghost collision
- PacMan and all ghosts reset to their starting positions after a collision
- A new countdown begins after the player loses a life
- Game restarts from the menu when all lives are lost
- A new round begins when all regular and power dots are collected
- High score is persisted in `record.txt` and displayed on-screen

### Invincibility/Power-Up Mode 🟡 Partially Done
- ✅ Power dots activate `invincible_mode` for a few seconds
- ✅ Ghosts switch to blue visual state while PacMan is invincible
- ✅ Ghosts move more slowly and target the opposite side of the board
- ✅ PacMan can eat ghosts while invincible, awarding +200 points per ghost
- ✅ Eaten ghosts reset to their spawn locations
- ❌ No full power-up UI, sound transitions, or advanced ghost-house/vulnerability effects

### Audio & Animations 🟡 Partially Done
- ✅ PacMan directional animation
- ✅ Idle PacMan animation
- ✅ Chomp sound effect
- ✅ Fruit/eat sound effect
- ✅ Blue ghost animation while invincible
- ❌ Power-up mode audio/animation polish
- ❌ Death/game-over sound effects
- ❌ Ghost mode-switch effects

### Menu System 🟡 Partially Done
- ✅ Start menu with PacMan logo
- ✅ Play button
- ✅ Decorative HUD artwork
- ✅ High score display in the HUD
- ❌ Game mode selection
- ❌ Sprite/character customization
- ❌ Settings/audio management

### Countdown & Round Management 🟡 Partially Done
- ✅ 4-second countdown before each round begins
- ✅ Staggered ghost release during a round
- ✅ Round reset after all dots are collected
- ✅ Score and lives reset when the player loses all lives
- ✅ High score tracking and display using `record.txt`
- ❌ Dedicated Game Over screen with final score

---

## Advanced Features

### Individual Ghost Features ✅ Done
- ✅ Four different ghost sprites: Blinky, Pinky, Inky, and Clyde
- ✅ Individual target behavior for each ghost
- ✅ Staggered ghost release
- ✅ Ghost collision bounce behavior
- ✅ Invincible-mode ghost behavior with blue ghosts and reverse targeting
- ✅ PacMan can eat ghosts during the power-dot state

### Unconventional Game Modes ❌ Remaining
- **Mode 1**: Firing mechanic for PacMan and/or ghosts
- **Mode 2**: Wall-jumping ability for PacMan and/or ghosts
- **Mode 3**: Escape portals (PacMan-only passage)
- **Mode 4**: Temporary invincibility, invisibility, or speed boosts for PacMan
- **Mode 5**: Reverse hunter mode (PacMan hunts ghosts before they escape)

---

## Project Statistics

| Component | Status | Notes |
|-----------|--------|-------|
| Core Gameplay | ✅ 95% | Fully playable with maze, scoring, lives, and four ghosts |
| Ghost System | ✅ 90% | All ghosts implemented with unique behaviors and invincible-mode logic |
| Feature Completeness | 🟡 80% | Most gameplay systems are complete; game-over UI and polish remain |
| Overall Progress | 🟡 85% | Solid playable build with remaining polish and advanced features |

---

## How to Build & Run

### Requirements
- RayLib library
- C compiler (gcc, clang, or MSVC)
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

On Windows, the repository also includes `main.exe`. Run it from the repository root so that the relative `assets/` paths resolve correctly.

---

## Game Controls

- **Arrow Keys**: Move PacMan (Up, Down, Left, Right)
- **Left Mouse Click**: Start the game from the menu
- **ESC**: Close the game window

---

## File Structure

```
PacMan/
├── main.c                         # Main game implementation
├── main.exe                       # Prebuilt Windows executable
├── record.txt                     # Stores the current highest score
├── raylib_template.code-workspace # VS Code workspace configuration
├── assets/                        # Game resources
│   ├── bg.png                     # Maze background
│   ├── idle.png                   # PacMan idle sprite
│   ├── pacman-logo.png            # Menu logo
│   ├── play-button.png            # Menu play button
│   ├── pacman-left/               # Left-facing PacMan animation frames
│   ├── pacman-right/              # Right-facing PacMan animation frames
│   ├── pacman-up/                 # Up-facing PacMan animation frames
│   ├── pacman-down/               # Down-facing PacMan animation frames
│   ├── ghosts/                    # Blinky, Pinky, Inky, Clyde, and blue invincible ghost assets
│   ├── other/                     # Apple, heart, and HUD textures
│   └── audio/                     # Sound effects
└── README.md                      # This file
```

---

## Next Steps

1. Add a dedicated Game Over screen
2. Improve power-up polish and display feedback
3. Add full ghost-house/vulnerability behavior and score balancing
4. Add more settings and audio controls
5. Implement game mode selection and customization
6. Refactor code into multiple files for better organization
7. Add more advanced gameplay modes and boss-style mechanics
