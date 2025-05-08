#include <SDL3/SDL.h>
#include <iostream>

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;
const int BOARD_SIZE = 3;
const int CELL_SIZE = WINDOW_WIDTH / BOARD_SIZE;

enum Player { NONE = 0, PLAYER_X, PLAYER_O };

Player board[BOARD_SIZE][BOARD_SIZE];
Player currentPlayer = PLAYER_X;

void SDL_RenderDrawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    const int diameter = (radius * 2);
    int x = (radius - 1);
    int y = 0;
    int tx = 1;
    int ty = 1;
    int error = (tx - diameter);

    while (x >= y) {
        // 8 octants of the circle
        SDL_RenderPoint(renderer, centerX + x, centerY - y);
        SDL_RenderPoint(renderer, centerX + x, centerY + y);
        SDL_RenderPoint(renderer, centerX - x, centerY - y);
        SDL_RenderPoint(renderer, centerX - x, centerY + y);
        SDL_RenderPoint(renderer, centerX + y, centerY - x);
        SDL_RenderPoint(renderer, centerX + y, centerY + x);
        SDL_RenderPoint(renderer, centerX - y, centerY - x);
        SDL_RenderPoint(renderer, centerX - y, centerY + x);

        if (error <= 0) {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0) {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

void drawBoard(SDL_Renderer* renderer) {
    // Draw grid lines
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // white
    for (int i = 1; i < BOARD_SIZE; i++) {
        // Vertical lines
        SDL_RenderLine(renderer, i * CELL_SIZE, 0, i * CELL_SIZE, WINDOW_HEIGHT);
        // Horizontal lines
        SDL_RenderLine(renderer, 0, i * CELL_SIZE, WINDOW_WIDTH, i * CELL_SIZE);
    }

    // Draw X's and O's
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            int x = i * CELL_SIZE;
            int y = j * CELL_SIZE;
            if (board[i][j] == PLAYER_X) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red for X
                // Draw X using thicker lines
                for(int d = -2; d <= 2; d++) {
                    SDL_RenderLine(renderer, x + 25, y + 25 + d, x + CELL_SIZE - 25, y + CELL_SIZE - 25 + d);
                    SDL_RenderLine(renderer, x + 25 + d, y + CELL_SIZE - 25, x + CELL_SIZE - 25 + d, y + 25);
                }
            } else if (board[i][j] == PLAYER_O) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);  // Blue for O
                for(int r = -2; r <= 2; r++) {
                    SDL_RenderDrawCircle(renderer, x + CELL_SIZE / 2, y + CELL_SIZE / 2, CELL_SIZE / 3 + r);
                }
            }
        }
    }
}

bool checkWin() {
    // Check rows and columns
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i][0] == currentPlayer && board[i][1] == currentPlayer && board[i][2] == currentPlayer)
            return true;
        if (board[0][i] == currentPlayer && board[1][i] == currentPlayer && board[2][i] == currentPlayer)
            return true;
    }
    // Check diagonals
    if (board[0][0] == currentPlayer && board[1][1] == currentPlayer && board[2][2] == currentPlayer)
        return true;
    if (board[0][2] == currentPlayer && board[1][1] == currentPlayer && board[2][0] == currentPlayer)
        return true;

    return false;
}

bool checkDraw() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == NONE) {
                return false;
            }
        }
    }
    return true;
}

void handleClick(int x, int y) {
    int row = x / CELL_SIZE;
    int col = y / CELL_SIZE;

    if (board[row][col] == NONE) {
        board[row][col] = currentPlayer;
        if (checkWin()) {
            std::cout << (currentPlayer == PLAYER_X ? "Player X wins!" : "Player O wins!") << std::endl;
        } else if (checkDraw()) {
            std::cout << "It's a draw!" << std::endl;
        } else {
            currentPlayer = (currentPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;  // Switch player
        }
    }
}

int main() {
    // if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    //     std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    //     return -1;
    // }

    SDL_Window* window = SDL_CreateWindow("Tic-Tac-Toe",  WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    bool quit = false;
    SDL_Event e;

    // Initialize the board
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = NONE;
        }
    }

    // Game loop
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            } else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    handleClick(e.button.x, e.button.y);
                }
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Black background
        SDL_RenderClear(renderer);

        // Draw board
        drawBoard(renderer);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
