#ifndef OTHELLO
#define OTHELLO

#define BOARD_HEIGHT 8
#define BOARD_WIDTH 8
#define INT_MIN -3.402823466e38
#define INT_MAX 3.402823466e38

#include "matrix.h"

typedef struct pos {
    int x;
    int y;
} pos;

typedef struct valuation {
    pos p;
    float v;
} valuation;

typedef struct agent {
    matrix W1; // 64 x 64 --> Matrice
    matrix W2; // 64 x 64 --> Matrice
    matrix W3; // 1 x 64  --> Ligne
    matrix b1; // 64 x 1  --> Colonne
    matrix b2; // 64 x 1  --> Colonne
    matrix b3; // 1 x 1   --> Colonne
} agent;

// Utilities
pos po(int x, int y);
float max(float a, float b);
float min(float a, float b);
// float abs(float a);
// int absi(int a);

// Main functions
int** initialize_game();
int** copy_board(int** board);
void free_board(int** board);
void start_game(int**);
pos* possible_moves(int**, int);
void print_positions(pos* position);
void place_stone(int**, pos, int);
void printBoard(int**);
valuation alpha_beta(int** board, float alpha, float beta, int recursion_left, int player, int turn, agent a);

agent create_agent();
void evaluate_two_agents(agent agent1, agent agent2);

#endif