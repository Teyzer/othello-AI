#ifndef OTHELLO
#define OTHELLO

#define BOARD_HEIGHT 8
#define BOARD_WIDTH 8
#define INT_MIN -2147483648
#define INT_MAX 2147483647

typedef struct pos {
    int x;
    int y;
} pos;

typedef struct valuation {
    pos p;
    int v;
} valuation;

// Utilities
pos po(int x, int y);
int max(int a, int b);
int min(int a, int b);
int abs(int a);

// Main functions
int** initialize_game();
int** copy_board(int** board);
void free_board(int** board);
void start_game(int**);
pos* possible_moves(int**, int);
void print_positions(pos* position);
void place_stone(int**, pos, int);
void printBoard(int**);
valuation alpha_beta(int** board, int alpha, int beta, int recursion_left, int player, int turn);


#endif