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
- Ghost-house gate handling in ghost movement

### Main Character Movement & Collision Management ✅ Done

- Smooth frame-time-based movement in four directions
- Arrow-key controls for left, right, up, and down
- Directional PacMan animation with three frames for each direction
- Idle sprite when PacMan is not moving
- Wall collision handling at grid positions
- PacMan movement begins after the four-second round countdown
- Downward movement is prevented from crossing the ghost-house gate

### Ghost AI & Movement ✅ Done

- **Blinky (Red Ghost)** ✅ Implemented
  - Uses PacMan's position as its chase target
  - Starts moving after the countdown
- **Pinky (Pink Ghost)** ✅ Implemented
  - Targets a position ahead of PacMan's current movement
  - Starts moving after Blinky
- **Inky (Cyan Ghost)** ✅ Implemented
  - Calculates a target from PacMan's and Blinky's positions
  - Starts moving after Pinky
- **Clyde (Orange Ghost)** ✅ Implemented
  - Chases PacMan when it is far from Blinky
  - Moves toward the lower-left target area when close to Blinky
- Shared `ghost_direction` function for decision-point navigation
- Ghosts avoid walls and the ghost-house gate where required
- Ghosts wrap around the horizontal tunnel
- Shared `ghost_bounce` function reverses two ghosts' speeds and directions after ghost-to-ghost collisions
- Ghost release is staggered during each round
- Ghost speed is controlled by a round difficulty coefficient, starting at 0.8 and increasing after completed rounds up to 1.0

### Dot Collection & Point Management ✅ Done

- Regular dots: 240 total, worth 10 points each
- Large power dots: 4 total, worth 50 points each
- Apple: worth 200 points
- Current score displayed during gameplay
- Highest score displayed during gameplay when `record.txt` is available
- Regular dots disappear after collection and play the chomp sound
- Power dots disappear after collection, play the fruit/eat sound, and activate invincibility
- Apple appears at a fixed maze position after the regular-dot count reaches 150 or fewer
- Apple disappears after eight seconds if it is not collected

### Lives, Game Over & Respawn ✅ Done

- Three-life system with heart icons in the HUD
- Collision detection between PacMan and all four ghosts
- Normal ghost collision removes one life
- Losing a life resets PacMan and all ghosts to their starting positions
- A new countdown starts after a normal collision
- When all lives are lost, the game enters a dedicated Game Over screen
- Game Over screen shows the current score and highest score
- New highest scores are written to `record.txt`
- Game Over screen provides **Menu**, **Restart**, and **Exit** buttons
- Clearing all regular and power dots starts a new round

### Invincibility/Power-Up Mode ✅ Done

- Eating a large power dot activates `invincible_mode`
- Invincibility lasts for five seconds
- Ghosts are rendered with the blue ghost texture during invincibility
- Blue ghost rendering fades using the game's animated alpha value
- Ghost movement is slower during invincibility
- Ghosts target the negative PacMan position instead of chasing PacMan normally
- PacMan can eat a ghost during invincibility
- Eating a ghost awards 200 points and plays the fruit/eat sound
- Eaten ghosts are reset to their individual starting positions
- A collision with a ghost does not remove a life while invincibility is active

### Audio & Animations 🟡 Partially Done

- ✅ PacMan directional animation
- ✅ Idle PacMan sprite
- ✅ Pulsing large power-dot rendering
- ✅ Blue ghost rendering during invincibility
- ✅ Fading invincible-ghost visual effect
- ✅ Chomp sound effect for regular dots
- ✅ Fruit/eat sound effect for power dots, apples, and eaten ghosts
- ❌ Dedicated death sound effect
- ❌ Dedicated game-over sound effect
- ❌ Dedicated power-up sound and warning effect

### Menu & Instructions System ✅ Done

- Start menu with PacMan logo
- Play button
- Decorative HUD artwork
- **HOW TO PLAY** button
- Instructions screen using `assets/howtoplay.png`
- Return button from the instructions screen
- Dedicated Game Over screen
- Menu, Restart, and Exit buttons on the Game Over screen

### Countdown & Round Management ✅ Done

- Four-second countdown before each round
- `3`, `2`, `1`, and `READY` countdown display
- Staggered ghost release during a round
- Round restart after all regular and power dots are collected
- Ghost speed coefficient increases by 0.05 after a completed round until it reaches 1.0
- Game Over state after all three lives are lost
- Menu and Restart actions reset the life and score state

---

## Advanced Features

### Individual Ghost Features ✅ Done

- Four ghost sprites: Blinky, Pinky, Inky, and Clyde
- Individual target calculations for all four ghosts
- Staggered release timing
- Ghost-to-ghost collision bounce behavior
- Horizontal tunnel movement
- Blue shared ghost appearance during invincibility
- Reverse-target behavior during invincibility
- PacMan can eat and reset ghosts during invincibility

### Unconventional Game Modes ❌ Remaining

- **Mode 1**: Firing mechanic for PacMan and/or ghosts
- **Mode 2**: Wall-jumping ability for PacMan and/or ghosts
- **Mode 3**: Escape portals for PacMan
- **Mode 4**: Additional special abilities beyond the current power-dot invincibility
- **Mode 5**: Reverse hunter mode

---

## Project Statistics

| Component | Status | Notes |
|-----------|--------|-------|
| Core Gameplay | ✅ 100% | Maze, movement, scoring, lives, four ghosts, power dots, and round progression are implemented |
| Ghost System | ✅ 95% | Four ghosts have individual targets, staggered release, bounce behavior, difficulty scaling, and invincibility interactions |
| Menu & Game Flow | ✅ 95% | Start menu, instructions, Game Over, Menu, Restart, and Exit flows are implemented |
| Feature Completeness | 🟡 90% | Core gameplay is complete; audio polish, settings, refactoring, and unconventional modes remain |
| Overall Progress | 🟡 90% | Fully playable build with a small number of planned polish and extension features remaining |

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

On Windows, the repository includes `main.exe`. Run the executable from the repository root so that the relative `assets/` paths and `record.txt` resolve correctly.

---

## Game Controls

- **Arrow Keys**: Move PacMan
- **Left Mouse Click**: Select Play, HOW TO PLAY, Return, Menu, Restart, or Exit buttons
- **ESC**: Close the game window

The complete control guide is available through the **HOW TO PLAY** button on the main menu.

---

## File Structure

```
PacMan/
├── main.c                         # Maze, game loop, movement, scoring, ghost AI, menus, and rendering
├── main.exe                       # Prebuilt Windows executable
├── record.txt                     # Persistent highest-score record
├── raylib_template.code-workspace # VS Code workspace configuration
├── assets/                        # Game resources
│   ├── bg.png                     # Maze background
│   ├── howtoplay.png              # Instructions screen
│   ├── idle.png                   # PacMan idle sprite
│   ├── pacman-logo.png            # Menu logo
│   ├── play-button.png            # Menu play button
│   ├── pacman-left/               # Left-facing PacMan animation frames
│   ├── pacman-right/              # Right-facing PacMan animation frames
│   ├── pacman-up/                 # Up-facing PacMan animation frames
│   ├── pacman-down/               # Down-facing PacMan animation frames
│   ├── ghosts/                    # Blinky, Pinky, Inky, Clyde, and blue ghost sprites
│   ├── other/                     # Apple, heart, and HUD textures
│   └── audio/                     # Chomp and fruit/eat sound effects
└── README.md                      # This file
```

---

## Implementation Notes

- The game is currently implemented in a single `main.c` file.
- RayLib and Raymath provide the window, texture, audio, vector, animation, and collision functionality.
- The maze uses `#` for walls, `.` for regular dots, `o` for large power dots, `-` for the ghost-house gate, and runtime markers for collected or temporary items.
- Ghost decision points are marked with `d` in the separate `decision` grid.
- Ghost movement uses directional target comparisons at decision points rather than a full shortest-path search.
- PacMan's base speed is 250 and ghost speed is scaled by the current `coeff` value.
- The game loads all assets using relative paths, so the working directory must contain the `assets` directory.
- The high score is read from `record.txt` when a round is initialized and updated on the Game Over screen when the current score reaches or exceeds the stored score.
- The code includes an image-source comment referencing the artwork source used by the project.

---

## Next Steps

1. Add dedicated power-up, death, and game-over audio
2. Add a power-up expiration warning or flashing ghost state
3. Improve ghost-house return behavior after a ghost is eaten
4. Add settings and configurable audio controls
5. Refactor the single source file into multiple modules
6. Implement unconventional game modes and additional challenge mechanics
