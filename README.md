### Project Description: Color Version of the 2048 Game for DE1-SoC Board

This project is a visually enhanced implementation of the classic **2048 game**, specifically designed to run on the **DE1-SoC development board**. The game incorporates vibrant colors and utilizes hardware-specific features like the VGA interface and character buffer, creating an interactive and engaging gaming experience. The color-coded tiles bring a modern and dynamic twist to the game while maintaining its original logic and gameplay.

---

#### **Key Features**:

1. **Color-Coded Tiles**:
   - Each tile in the game grid is represented with unique colors based on its value.
   - The colors range from light gray for empty tiles to bright hues for higher values like gold for 1024 and bright orange for 2048, providing a visually appealing experience.

2. **Grid Design**:
   - The grid consists of a 4x4 matrix displayed on a VGA screen connected to the DE1-SoC board.
   - Tiles are drawn using filled rectangles with appropriate spacing for a clean and organized layout.

3. **Dynamic Text Rendering**:
   - Numbers on the tiles are rendered using the character buffer, allowing the display of multi-digit values.
   - The game dynamically updates the numbers on the screen as tiles merge and new ones appear.

4. **Interactive Gameplay**:
   - Players can control the game using pushbutton keys on the DE1-SoC board.
   - Movement directions (left, right, up, down) are mapped to the board's keys, enabling smooth sliding and merging of tiles.

5. **Random Tile Addition**:
   - After each move, a new tile (either 2 or 4) is added to a random empty cell.
   - There is a 10% chance for a tile of value 4 to appear, adding an element of unpredictability.

6. **Game State Management**:
   - The program checks for winning conditions (reaching target color) and game-over states (no valid moves).
   - Messages like "WIN!" or "GAME OVER" are displayed on the VGA screen upon reaching these states.

7. **Efficient Algorithms**:
   - **Tile Sliding and Combining**: Implements algorithms to slide and merge tiles in all four directions efficiently.
   - **Grid Manipulation**: Includes functions for transposing and reversing the grid, simplifying operations like vertical movements.

8. **VGA Integration**:
   - The game uses direct memory-mapped VGA operations for pixel rendering, ensuring seamless and fast graphics updates.
   - Backgrounds, tiles, and text are drawn directly on the VGA screen.

9. **Hardware-Specific Implementation**:
   - Memory-mapped I/O is used for VGA rendering and character display.
   - The game leverages the DE1-SoC’s pushbutton keys for user input and on-screen updates.

---

#### **Technical Details**:

1. **Tile Colors**:
   - Each tile value corresponds to a unique 16-bit color code, ensuring visually distinct representations.

2. **Game Grid**:
   - The 4x4 grid is implemented as a 2D array, storing integer values for tile states.

3. **User Input**:
   - The `KEY` register of the DE1-SoC is polled to detect button presses for directional inputs.

4. **Graphics Drawing**:
   - Functions like `write_pixel()` and `draw_rectangle()` are used for rendering, directly interfacing with the VGA buffer.

5. **Character Buffer**:
   - Numbers and messages are displayed using the character buffer, which writes characters to specific screen coordinates.

6. **Game Initialization**:
   - The game starts with an empty grid and two random tiles, ensuring a consistent starting point.

7. **Random Tile Placement**:
   - The program identifies all empty cells and places a new tile in a randomly chosen position.

8. **Endgame Conditions**:
   - The game detects when no moves are possible or when the target color is reached.

---

#### **How It Works**:

1. **Startup**:
   - The game initializes the grid, places two tiles randomly, and renders the initial state on the screen.

2. **Gameplay Loop**:
   - The player presses a key to move the tiles in the desired direction.
   - The program processes the move, sliding and combining tiles as necessary.
   - A new tile is added to the grid, and the game state is updated.

3. **Win or Lose**:
   - If the player reaches a tile with the value of 2048, the game displays a "WIN!" message.
   - If no valid moves are available, the game ends with a "GAME OVER" message.

---
#### **Usage**:
Run the code on the CPulator NIOS-II Emulator or an FPGA board with a VGA output to experience the color version of 2048 game in a hardware-driven environment.

#### **Potential Enhancements**:
1. **Dynamic Grid Size**: Expand the game to support grids larger than 4x4.
2. **Improved Input Mechanism**: Add support for external controllers or keyboard inputs.
3. **Score Tracking**: Implement a scoring system displayed on the screen.
4. **Pause and Resume**: Allow players to pause and resume the game.

---

This project highlights the integration of hardware and software to create a polished, colorful rendition of the 2048 game. It serves as an excellent demonstration of using embedded systems for interactive applications, blending algorithmic logic with hardware-specific rendering techniques.
