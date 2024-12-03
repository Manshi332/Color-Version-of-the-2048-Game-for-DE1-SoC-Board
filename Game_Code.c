#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define SIZE 4
#define WIN_VALUE 2048
#define TILE_SIZE 40
#define GRID_OFFSET_X 60
#define GRID_OFFSET_Y 40
#define GRID_SPACING 5

int grid[SIZE][SIZE];

// Function to write a pixel to the screen
void write_pixel(int x, int y, short colour) {
    volatile short *vga_addr = (volatile short*)(0x08000000 + (y<<10) + (x<<1));
    *vga_addr = colour;
}

// Function to clear the screen
void clear_screen() {
    int x, y;
    for (x = 0; x < 320; x++) {
        for (y = 0; y < 240; y++) {
            write_pixel(x, y, 0);
        }
    }
}

// Function to write character
void write_char(int x, int y, char c) {
    volatile char * character_buffer = (char *) (0x09000000 + (y<<7) + x);
    *character_buffer = c;
}

// Function to draw a filled rectangle
void draw_rectangle(int x, int y, int width, int height, short colour) {
    for (int i = x; i < x + width && i < 320; i++) {
        for (int j = y; j < y + height && j < 240; j++) {
            write_pixel(i, j, colour);
        }
    }
}

// Function to get tile color based on value
short get_tile_color(int value) {
    switch(value) {
        case 0: return 0x7BEF;     // Light gray
        case 2: return 0xFFE0;     // Yellow
        case 4: return 0xFE00;     // Orange
        case 8: return 0xF800;     // Red
        case 16: return 0x07E0;    // Green
        case 32: return 0x001F;    // Blue
        case 64: return 0x780F;    // Purple
        case 128: return 0xF81F;   // Pink
        case 256: return 0x07FF;   // Cyan
        case 512: return 0xFFFF;   // White
        case 1024: return 0xFD20;  // Gold
        case 2048: return 0xFC00;  // Bright Orange
        default: return 0x0000;    // Black
    }
}

// Function to convert integer to string
void int_to_str(int num, char* str) {
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    
    int i = 0;
    while (num > 0) {
        str[i++] = '0' + (num % 10);
        num /= 10;
    }
    str[i] = '\0';
    
    // Reverse the string
    for (int j = 0; j < i/2; j++) {
        char temp = str[j];
        str[j] = str[i-1-j];
        str[i-1-j] = temp;
    }
}

// Draw the game grid
void draw_grid() {
    clear_screen();
    
    // Draw background
    draw_rectangle(GRID_OFFSET_X - GRID_SPACING, 
                  GRID_OFFSET_Y - GRID_SPACING,
                  (TILE_SIZE * SIZE) + (GRID_SPACING * 2),
                  (TILE_SIZE * SIZE) + (GRID_SPACING * 2),
                  0x7BEF);  // Light gray background

    // Draw tiles
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int x = GRID_OFFSET_X + (j * TILE_SIZE);
            int y = GRID_OFFSET_Y + (i * TILE_SIZE);
            
            // Draw tile background
            draw_rectangle(x, y, TILE_SIZE - GRID_SPACING, TILE_SIZE - GRID_SPACING, 
                         get_tile_color(grid[i][j]));
            
            // Draw number
            if (grid[i][j] != 0) {
                char number[5];
                int_to_str(grid[i][j], number);
                int text_x = x/4 + 1;  // Adjust for character buffer coordinates
                int text_y = y/4;      // Adjust for character buffer coordinates
                
                // Write each character of the number
                for (int k = 0; number[k] != '\0'; k++) {
                    write_char(text_x + k, text_y, number[k]);
                }
            }
        }
    }
}

// Initialize the game grid
void initialize_game() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            grid[i][j] = 0;
        }
    }
}

// Add a random tile (2 or 4) to an empty cell
void add_random_tile() {
    int empty_cells[SIZE * SIZE][2];
    int count = 0;

    // Find all empty cells
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (grid[i][j] == 0) {
                empty_cells[count][0] = i;
                empty_cells[count][1] = j;
                count++;
            }
        }
    }

    // If there are empty cells, add a new tile
    if (count > 0) {
        int rand_index = rand() % count;
        int row = empty_cells[rand_index][0];
        int col = empty_cells[rand_index][1];
        grid[row][col] = (rand() % 10 == 0) ? 4 : 2;  // 10% chance of 4, 90% chance of 2
    }
}

// Check for win condition
bool check_win() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (grid[i][j] == WIN_VALUE) {
                return true;
            }
        }
    }
    return false;
}

// Check for game over condition
bool check_game_over() {
    // Check for empty cells
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (grid[i][j] == 0) {
                return false;
            }
        }
    }
    
    // Check for possible merges
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE - 1; j++) {
            if (grid[i][j] == grid[i][j + 1]) {
                return false;
            }
        }
    }
    for (int j = 0; j < SIZE; j++) {
        for (int i = 0; i < SIZE - 1; i++) {
            if (grid[i][j] == grid[i + 1][j]) {
                return false;
            }
        }
    }
    
    return true;
}

// Slide and combine tiles in a single row
void slide_and_combine_row(int row[SIZE]) {
    int temp[SIZE] = {0};  // Temporary array to store the result
    int index = 0;

    // Slide non-zero elements to the left
    for (int i = 0; i < SIZE; i++) {
        if (row[i] != 0) {
            temp[index++] = row[i];
        }
    }

    // Combine adjacent tiles of the same value
    for (int i = 0; i < SIZE - 1; i++) {
        if (temp[i] == temp[i + 1] && temp[i] != 0) {
            temp[i] *= 2;
            temp[i + 1] = 0;
        }
    }

    // Slide again to remove any gaps created by combining
    index = 0;
    for (int i = 0; i < SIZE; i++) {
        if (temp[i] != 0) {
            row[index++] = temp[i];
        } else {
            row[index++] = 0;
        }
    }
}

// Slide and combine tiles in the grid for a given direction
void slide_and_combine_grid(int direction) {
    int temp[SIZE];

    switch (direction) {
        case 0:  // Left
            for (int i = 0; i < SIZE; i++) {
                slide_and_combine_row(grid[i]);
            }
            break;

        case 1:  // Right
            for (int i = 0; i < SIZE; i++) {
                // Reverse row, slide and combine, then reverse back
                for (int j = 0; j < SIZE; j++) {
                    temp[j] = grid[i][SIZE - 1 - j];
                }
                slide_and_combine_row(temp);
                for (int j = 0; j < SIZE; j++) {
                    grid[i][SIZE - 1 - j] = temp[j];
                }
            }
            break;

        case 2:  // Up
            for (int j = 0; j < SIZE; j++) {
                for (int i = 0; i < SIZE; i++) {
                    temp[i] = grid[i][j];
                }
                slide_and_combine_row(temp);
                for (int i = 0; i < SIZE; i++) {
                    grid[i][j] = temp[i];
                }
            }
            break;

        case 3:  // Down
            for (int j = 0; j < SIZE; j++) {
                for (int i = 0; i < SIZE; i++) {
                    temp[i] = grid[SIZE - 1 - i][j];
                }
                slide_and_combine_row(temp);
                for (int i = 0; i < SIZE; i++) {
                    grid[SIZE - 1 - i][j] = temp[i];
                }
            }
            break;
    }
}

// Main game loop
void play_game() {
    srand(time(NULL));
    initialize_game();
    add_random_tile();
    add_random_tile();
    draw_grid();

    volatile int * KEY_ptr = (int *) 0xFF200050;  // Pointer to the pushbutton key register
    int prev_key = 0, key_data;

    while (true) {
        key_data = *(KEY_ptr);

        if (key_data != prev_key) {
            if (key_data & 0x1) {
                slide_and_combine_grid(0);  // Left
            } else if (key_data & 0x2) {
                slide_and_combine_grid(1);  // Right
            } else if (key_data & 0x4) {
                slide_and_combine_grid(2);  // Up
            } else if (key_data & 0x8) {
                slide_and_combine_grid(3);  // Down
            }

            add_random_tile();
            draw_grid();

            if (check_win()) {
                write_char(35, 30, 'W');
                write_char(36, 30, 'I');
                write_char(37, 30, 'N');
                write_char(38, 30, '!');
                break;
            }
            if (check_game_over()) {
                write_char(35, 30, 'G');
                write_char(36, 30, 'A');
                write_char(37, 30, 'M');
                write_char(38, 30, 'E');
                write_char(39, 30, 'O');
                write_char(40, 30, 'V');
                write_char(41, 30, 'E');
                write_char(42, 30, 'R');
                break;
            }
        }
        prev_key = key_data;
    }
}



// Transpose the grid (swap rows and columns)
void transpose_grid() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = i + 1; j < SIZE; j++) {
            int temp = grid[i][j];
            grid[i][j] = grid[j][i];
            grid[j][i] = temp;
        }
    }
}

// Reverse the rows of the grid
void reverse_grid() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE / 2; j++) {
            int temp = grid[i][j];
            grid[i][j] = grid[i][SIZE - 1 - j];
            grid[i][SIZE - 1 - j] = temp;
        }
    }
}


int main(void) {
    play_game();
    return 0;
}
