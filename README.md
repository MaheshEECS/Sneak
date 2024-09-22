# ComputerArchitecture (Sneak)
# Snakes Game Overview

The Snakes game is a grid-based game where players control snakes navigating through a board filled with walls, fruits, and other snakes. Each snake is represented by specific characters, indicating its direction and state. The game mechanics include moving the snake in its current direction, eating fruits to grow, and dying upon colliding with walls or other snakes.

### Key Features

- **Game Board**: A grid of characters with walls (`#`), empty spaces, fruits (`*`), and snake representations.
- **Snake Movement**: Snakes move according to their head direction, can grow by eating fruits, and die upon collisions.
- **Snake Numbering**: Snakes are numbered based on their tail positions, which remain constant throughout the game.

### Data Structures

The game state is managed using the `game_state_t` struct, which holds the board layout, the number of snakes, and their positions. Each snake's properties are encapsulated in the `snake_t` struct.

This project involves implementing the game logic, managing the snakes' behaviors, and ensuring a smooth gameplay experience.

