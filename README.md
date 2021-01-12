# ConwaysGameOfLife
The Game of Life, also known simply as Life, is a cellular automaton devised by the British mathematician John Horton Conway in 1970.[1] It is a zero-player game, meaning that its evolution is determined by its initial state, requiring no further input. One interacts with the Game of Life by creating an initial configuration and observing how it evolves. It is Turing complete and can simulate a universal constructor or any other Turing machine. (Wikipedia: https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)

## Game Rules
1- Any live cell with fewer than two live neighbours dies, as if by underpopulation.
2- Any live cell with two or three live neighbours lives on to the next generation.
3- Any live cell with more than three live neighbours dies, as if by overpopulation.
4- Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.

## Simulation
The simulation made using SFML (Simple and Fast Multimedia Library) based on Open-GL. The size was fixed to be 50 x 50 yet it can be of any size.
### Controls
1- Use ```Space``` to Start / Stop the simulation at any given moment (it starts freazed).
2- Use the ```Arrow Keys``` to move around the grid and see all the cells.
3- Use the ```Left Mous Button``` to make the cell either alive or dead.

### Compiling
1- You need to download SFML from this link: https://www.sfml-dev.org/download.php
2- Put the package in the solution folder of VisualStudio
3- No textures needed for compilation.
