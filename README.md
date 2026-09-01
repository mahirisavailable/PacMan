# PacMan

A RayLib-based recreation of the classic PacMan game for CSE-102, BUET 1-1.

## Developers
- Mahir Ahmed
- Raihan Biswas

---

## Core Features

### Maze & Grid System ✅ Done
- 31x28 grid-based maze layout using a 2D character array
- Classic PacMan maze design with walls, corridors, and spawn zones
- Walls block both player and ghost movement

### Main Character Movement & Collision Management ✅ Done
- Smooth directional movement (up, down, left, right)
- Directional sprite animations for each movement direction
- Collision detection with maze walls
- Smooth directional transitions using velocity-based movement system

### Ghost AI & Movement 🟡 Partially Done
- **Blinky (Red Ghost)** ✅ Implemented with chase AI
  - Pursues PacMan after 4-second countdown
  - Navigates maze with wall collision detection
  - Teleportation at maze edges
- **Pinky, Inky, Clyde** ❌ Not implemented
  - Need to be added with unique AI patterns

### Dot Collection & Point Management ✅ Done
- Regular dots (240 total): +10 points each
- Large power dots (4 total): +50 points each
- Special fruit (apple): +200 points when collected
- Running score display

### Death, Game Over & Respawn ✅ Done
- 3-life system with heart display
- Collision detection between PacMan and ghosts
- Ghost and player respawn to starting positions on collision
- Game restarts when all lives are lost

### Invincibility/Power-Up Mode ❌ Remaining
- Power-up mechanic when PacMan eats large dots
- Ability for PacMan to eat ghosts during power-up
- Ghost vulnerability state and respawn in ghost house
- Mode switch visual/audio effects

### Audio & Animations 🟡 Partially Done
- ✅ Chomp sound effect (dot collection)
- ✅ Fruit eating sound effect
- ❌ Power-up mode audio/animations
- ❌ Death/game over sound effects
- ❌ Ghost mode switch effects

### Menu System 🟡 Partially Done
- ✅ Start menu with PacMan logo and play button
- ❌ Game mode selection
- ❌ Sprite/character customization
- ❌ Settings/audio management

### Countdown & Round Management 🟡 Partially Done
- ✅ 4-second countdown before each round begins
- ❌ Dedicated Game Over screen with final score
- ❌ High score tracking and display

---

## Advanced Features

### Individual Ghost Features ❌ Remaining
- Unique AI patterns for each of the 4 ghosts
- Different colors and sprites for each ghost
- Custom movement behaviors

### Unconventional Game Modes ❌ Remaining
- **Mode 1**: Firing mechanic for PacMan and/or ghosts
- **Mode 2**: Wall-jumping ability for PacMan and/or ghosts
- **Mode 3**: Escape portals (PacMan-only passage)
- **Mode 4**: Temporary invincibility/invisibility/speed boosts for PacMan
- **Mode 5**: Reverse hunter mode (PacMan hunts ghosts before they escape)

---

## Project Statistics

| Component | Status | Notes |
|-----------|--------|-------|
| Core Gameplay | ✅ 70% | Fully playable with 1 ghost |
| Feature Completeness | 🟡 50% | Core features done, missing ghosts & power-ups |
| Overall Progress | 🟡 55% | Solid foundation, needs polish & remaining ghosts |

---

## How to Build & Run

### Requirements
- RayLib library
- C compiler (gcc, clang, or MSVC)

### Build
```bash
gcc main.c -o main -lraylib -lm
```

### Run
```bash
./main
```

---

## Game Controls

- **Arrow Keys**: Move PacMan (Up, Down, Left, Right)
- **Left Mouse Click**: Start game from menu
- **ESC**: Close game window

---

## File Structure

```
PacMan/
├── main.c                    # Main game implementation
├── assets/                   # Game resources
│   ├── pacman-*.png          # PacMan animations
│   ├── ghosts/               # Ghost sprites
│   ├── other/                # Collectibles and UI elements
│   └── audio/                # Sound effects
└── README.md                 # This file
```

---

## Next Steps

1. Implement 3 additional ghosts (Pinky, Inky, Clyde) with unique AI patterns
2. Add power-up mode mechanics
3. Create a proper Game Over screen
4. Refactor code into multiple files for better organization
5. Implement game mode selection menu
6. Add high score persistence
7. Implement unconventional game modes

