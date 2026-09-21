# PacMan

A C and raylib recreation of the classic Pac-Man game developed for the BUET CSE 1-1 project. The game features a grid-based maze, animated Pac-Man movement, four ghosts with distinct targeting behavior, collectibles, scoring, lives, audio, and a start menu.

## Developers

- Mahir Ahmed
- Raihan Biswas

## Current Features

### Maze and movement

- 31×28 character-grid maze implemented directly in `main.c`
- Wall, corridor, ghost-house, and tunnel layout
- Smooth, frame-time-based Pac-Man movement
- Direction changes are accepted at grid intersections when the requested path is open
- Horizontal tunnel wrap-around at both maze edges
- Directional Pac-Man animation with three frames per direction
- Idle Pac-Man sprite when the player is stationary

### Collectibles and scoring

| Collectible | Points | Behavior |
|---|---:|---|
| Small dot | 10 | Removed from the maze and plays the chomp sound |
| Power dot | 50 | Removed from the maze and plays the fruit/eat sound |
| Apple | 200 | Appears after enough small dots have been collected and remains available for a limited time |

The game tracks 240 small dots and four power dots. An apple appears at a fixed maze position after the small-dot count reaches 150 or fewer, stays visible for eight seconds, and can be collected once.

### Four ghost system

All four ghosts are present and rendered with their own sprites:

- **Blinky (red):** targets Pac-Man during chase phases
- **Pinky (pink):** targets a position ahead of Pac-Man based on the current movement vector
- **Inky (cyan):** calculates a target using Pac-Man's position and Blinky's position
- **Clyde (orange):** chases Pac-Man when far away and retreats toward the lower-left corner when close to Blinky

Ghost movement uses the shared `ghost_direction` function. It evaluates available maze paths at decision points, avoids walls and the ghost-house gate where appropriate, and follows each ghost's current target. Ghosts are released in stages after the round countdown:

- Blinky: immediately after the countdown
- Pinky: after 10 additional seconds
- Inky: after 20 additional seconds
- Clyde: after 30 additional seconds

Ghosts wrap around the horizontal tunnel and reverse direction when they collide. The repeated ghost-collision logic is centralized in the `ghost_bounce` function.

### Lives, collisions, and rounds

- Three lives are displayed with heart textures in the HUD
- A collision with any ghost removes one life
- After a collision, Pac-Man and all ghosts return to their starting positions and the round countdown restarts
- Losing all lives resets the score and starts a new game from the menu
- Clearing all small and power dots starts a new round
- A four-second countdown displays `3`, `2`, `1`, and `READY` before movement begins

### Menu, rendering, and audio

- Start menu with Pac-Man logo, play button, and decorative HUD artwork
- Score and remaining-life display during gameplay
- Maze background and animated power-dot rendering
- Chomp sound for small dots
- Eat-fruit sound for power dots and the apple
- Textures are unloaded and the audio/window systems are closed when the program exits

## Controls

- **Arrow keys:** Move Pac-Man
- **Left mouse button:** Start the game from the menu by clicking the play button
- **Esc:** Close the game window

## Requirements

- A C compiler such as GCC, Clang, or MSVC
- [raylib](https://www.raylib.com/)
- A desktop environment capable of opening a raylib window

The game loads resources using paths under `assets/`, so run the executable from the repository root or ensure the working directory contains the `assets` directory.

## Build and Run

### Linux

```bash
gcc main.c -o main -lraylib -lm
./main
```

Depending on how raylib is installed, additional platform libraries may be required by the linker.

### Windows

The repository includes a prebuilt `main.exe`. It can be run from the repository root so that the relative asset paths resolve correctly. To build from source, use a raylib-compatible GCC/MinGW or MSVC setup and link raylib with the platform libraries required by that installation.

## Project Structure

```text
PacMan/
├── main.c                         # Game loop, maze, gameplay, ghost AI, and rendering
├── main.exe                       # Prebuilt Windows executable
├── raylib_template.code-workspace # VS Code workspace configuration
├── assets/
│   ├── bg.png                     # Maze background
│   ├── idle.png                   # Pac-Man idle sprite
│   ├── pacman-logo.png            # Main-menu logo
│   ├── play-button.png            # Main-menu play button
│   ├── pacman-left/               # Three left-facing animation frames
│   ├── pacman-right/              # Three right-facing animation frames
│   ├── pacman-up/                 # Three upward-facing animation frames
│   ├── pacman-down/               # Three downward-facing animation frames
│   ├── ghosts/                    # Blinky, Pinky, Inky, and Clyde sprites
│   ├── other/                     # Apple, heart, and HUD artwork
│   └── audio/                     # Chomp and fruit/eat sound effects
└── README.md
```

## Implementation Notes

- The game is currently concentrated in `main.c`.
- Raylib's `Vector2` and collision helpers are used for movement, targeting, and collision detection.
- The maze uses `#` for walls, `.` for small dots, `o` for power dots, `-` for the ghost-house gate, and runtime markers for collected or temporary items.
- Ghost decision points are represented by a separate `decision` grid.
- Ghost target behavior is implemented as a chase/scatter-style system rather than a full reproduction of the original arcade game's pathfinding.

## Remaining Work

The current implementation does not yet include:

- Power mode after collecting a power dot
- Ghost vulnerability and eating ghosts
- A dedicated game-over screen or persistent high-score system
- Additional game-mode selection or character customization
- Settings and configurable audio controls
- Advanced or unconventional modes such as firing, wall-jumping, portals, or temporary speed/invisibility boosts
- Separation of the single source file into smaller modules

## License

No license has been specified for this repository yet.
