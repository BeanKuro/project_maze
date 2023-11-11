#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <time.h>

// Maze array
char** maze;
int rows, cols;

int initialPlayerX = 1, initialPlayerY = 1; // Initial player position
int playerX, playerY; // Current player position

const char* filename; // Global variable to store the current maze filename

clock_t startTime; // Variable to store the start time

void initializeMaze(const char* filename) {
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        printf("Error!! Opening file %s\n", filename);
        exit(1);
    }

    // Assuming a maximum maze size for simplicity
    char buffer[100][100];

    // Initialize rows and cols to zero
    rows = cols = 0;

    // Read the maze into the buffer
    while (fgets(buffer[rows], sizeof(buffer[0]), file) != NULL) {
        rows++;
    }

    fclose(file);

    // Allocate memory for maze based on the actual size
    maze = (char**)malloc(rows * sizeof(char*));
    for (int i = 0; i < rows; i++) {
        cols = strlen(buffer[i]);
        maze[i] = (char*)malloc(cols * sizeof(char) + 1);
        strcpy(maze[i], buffer[i]);
    }

    // Set the initial player position
    playerX = initialPlayerX;
    playerY = initialPlayerY;
}

void selectMaze(int sel) {
    switch (sel) {
    case 1:
        filename = "maze1.txt";
        break;
    case 2:
        filename = "maze2.txt";
        break;
    case 3:
        filename = "maze3.txt";
        break;
    default:
        printf("\nPlease input a valid number!");
        return;
    }

    initializeMaze(filename);
}

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void drawMaze() {
    system("cls");

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            gotoxy(j, i);

            if (i == playerY && j == playerX) {
                _putch('O');
            }
            else {
                _putch(maze[i][j]);
            }
        }
    }
}

void resetPlayerPosition() {
    // Reset player position to initial coordinates
    playerX = initialPlayerX;
    playerY = initialPlayerY;
}

void forfeitGame() {
    system("cls");
    printf("\nYou forfeited the game. Returning to the main menu.\n\n");
    resetPlayerPosition(); // Reset player position
    // Free memory
    for (int i = 0; i < rows; i++) {
        free(maze[i]);
    }
    free(maze);
}

void congratulateAndPrintTime() {
    clock_t endTime = clock();
    double totalTime = ((double)endTime - startTime) / ((clock_t)1000);
    system("cls");
    printf("\nCongratulations! You cleared the maze!");
    printf("\nTime taken: %.2f seconds\n\n", totalTime);
    resetPlayerPosition(); // Reset player position
}

int main() {
    B:
    int yn;
    do {
        printf("Hello, welcome to my MAZE!!");
        printf("\nDo you want to play this game? (1: Start / Other: Off): ");
        scanf("%d", &yn);

        char key;

        if (yn == 1) {
            printf("\nOk, the game is starting!");
            int sel;

        A:
            printf("\nSelect a maze difficulty! (input 1, 2, 3): ");

            // Check if scanf successfully read an integer
            if (scanf("%d", &sel) != 1) {
                printf("\nInvalid input. Please enter a number.");

                // Clear the input buffer
                while (getchar() != '\n');

                goto A;
            }

            switch (sel) {
            case 1:
            case 2:
            case 3:
                selectMaze(sel); // Call selectMaze after getting user input
                break;
            default:
                printf("\nPlease input a valid number!");
                goto A;
            }

            // Initialize the start time
            startTime = clock();

            while (1) {
                drawMaze();

                key = _getch();

                switch (key) {
                case 'w':
                    if (playerY > 0 && maze[playerY - 1][playerX] != '#') {
                        playerY--;
                    }
                    break;
                case 's':
                    if (playerY < rows - 1 && maze[playerY + 1][playerX] != '#') {
                        playerY++;
                    }
                    break;
                case 'a':
                    if (playerX > 0 && maze[playerY][playerX - 1] != '#') {
                        playerX--;
                    }
                    break;
                case 'd':
                    if (playerX < cols - 1 && maze[playerY][playerX + 1] != '#') {
                        playerX++;
                    }
                    break;
                case 'q':
                    forfeitGame(); // Call the forfeitGame function
                    goto B;
                default:
                    break;
                }

                // Check for reaching the exit '@'
                if (maze[playerY][playerX] == '@') {
                    congratulateAndPrintTime();
                    break; // Exit the loop and return to the main menu
                }
            }
        }
        else {
            printf("\n\nOK, the game is off.");
            break; // Exit the loop and end the program
        }

    } while (1);

    return 0;
}
