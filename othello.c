#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "othello.h"

/*

CONVENTIONS
VIDE: 0
BLANC: 1
NOIR: -1

*/

int max(int a, int b) {
    return a > b ? a : b;
}

int min(int a, int b) {
    return a < b ? a : b;
}

int abs(int a) {
    return a < 0 ? a : -a;
}

pos po(int x, int y) {
    pos r;
    r.x = x;
    r.y = y;
    return r;
}


int** initialize_game() {

    int** arr = (int**)malloc(sizeof(int*) * BOARD_HEIGHT);
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        int* temp = (int*)malloc(sizeof(int) * BOARD_WIDTH);
        for (int j = 0; j < BOARD_WIDTH; j++) {
            temp[j] = 0;
        }
        arr[i] = temp;
    }

    return arr;

}


int** copy_board(int** board) {

    int** arr = (int**)malloc(sizeof(int*) * BOARD_HEIGHT);
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        int* temp = (int*)malloc(sizeof(int) * BOARD_WIDTH);
        for (int j = 0; j < BOARD_WIDTH; j++) {
            temp[j] = board[i][j];
        }
        arr[i] = temp;
    }

    return arr;

}

void free_board(int** board) {
    
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        free(board[i]);
    }

    free(board);

}

void start_game(int** board) {
    board[3][3] = 1;
    board[4][3] = -1;
    board[3][4] = -1;
    board[4][4] = 1;
}

pos* possible_moves(int** current_board, int player) {

    int max_possible_moves = 30; // j'ai supposé qu'on aurait pas plus de 16 moves possibles
    pos* results = (pos*)malloc(sizeof(pos) * max_possible_moves);
    bool* allowed = (bool*)malloc(sizeof(bool) * BOARD_HEIGHT * BOARD_WIDTH);

    int current_index = 0, k;
    bool is_valid, found_other;

    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            
            if (current_board[i][j]) { // ie non égal à 0
                continue;
            }

            is_valid = false;

            // CHECK TOP
            k = 1;
            found_other = false;
            while (i - k >= 0) {
                if (current_board[i - k][j] == 0) {
                    break;
                } else if (current_board[i - k][j] == player) {
                    is_valid = is_valid || found_other;
                } else {
                    found_other = true;
                }
                k++;
            }

            // CHECK BOTTOM
            k = 1;
            found_other = false;
            while (i + k < BOARD_HEIGHT) {
                if (current_board[i + k][j] == 0) {
                    break;
                } else if (current_board[i + k][j] == player) {
                    is_valid = is_valid || found_other;
                } else {
                    found_other = true;
                }
                k++;
            }

            // CHECK LEFT
            k = 1;
            found_other = false;
            while (j - k >= 0) {
                if (current_board[i][j - k] == 0) {
                    break;
                } else if (current_board[i][j - k] == player) {
                    is_valid = is_valid || found_other;
                } else {
                    found_other = true;
                }
                k++;
            }

            // CHECK RIGHT
            k = 1;
            found_other = false;
            while (j + k < BOARD_WIDTH) {
                if (current_board[i][j + k] == 0) {
                    break;
                } else if (current_board[i][j + k] == player) {
                    is_valid = is_valid || found_other;
                } else {
                    found_other = true;
                }
                k++;
            }

            // CHECK TOP LEFT
            k = 1;
            found_other = false;
            while (i - k >= 0 && j - k >= 0) {
                if (current_board[i - k][j - k] == 0) {
                    break;
                } else if (current_board[i - k][j - k] == player) {
                    is_valid = is_valid || found_other;
                } else {
                    found_other = true;
                }
                k++;
            }

            // CHECK TOP RIGHT
            k = 1;
            found_other = false;
            while (i - k >= 0 && j + k < BOARD_WIDTH) {
                if (current_board[i - k][j + k] == 0) {
                    break;
                } else if (current_board[i - k][j + k] == player) {
                    is_valid = is_valid || found_other;
                } else {
                    found_other = true;
                }
                k++;
            }

            // CHECK BOTTOM LEFT
            k = 1;
            found_other = false;
            while (i + k < BOARD_HEIGHT && j - k >= 0) {
                if (current_board[i + k][j - k] == 0) {
                    break;
                } else if (current_board[i + k][j - k] == player) {
                    is_valid = is_valid || found_other;
                } else {
                    found_other = true;
                }
                k++;
            }

            // CHECK BOTTOM RIGHT
            k = 1;
            found_other = false;
            while (i + k < BOARD_HEIGHT && j + k < BOARD_WIDTH) {
                if (current_board[i + k][j + k] == 0) {
                    break;
                } else if (current_board[i + k][j + k] == player) {
                    is_valid = is_valid || found_other;
                } else {
                    found_other = true;
                }
                k++;
            }

            if (is_valid) {

                pos r;
                r.x = i;
                r.y = j;
                results[current_index] = r;
                current_index++;

            }

        }
    }

    pos end_pos;
    end_pos.x = -1;
    end_pos.y = -1;
    results[current_index] = end_pos;

    return results;

}

void print_positions(pos* position) {
    
    int index = 0;

    if (position[index].x == -1) {
        printf("[]");
        return;
    }

    printf("[");
    printf("(%d, %d)", position[index].x, position[index].y);
    index++;
    while (position[index].x != -1) {
        printf(", (%d, %d)", position[index].x, position[index].y);
        index++;
    }
    printf("]\n");

}



void place_stone(int** board, pos p, int colour){
    
    board[p.y][p.x] = colour;
    
    // FUCKED UP CODE COMING UP
    // BOT
    int i;
    for(i = 1; p.x + i < 8 && board[p.y][p.x + i] == -colour; i++){}
    if (p.x + i < 8 && board[p.y][p.x + i] == colour){
        for(int j = 1; j < i; j++){
            board[p.y][p.x + j] = colour;
        }
    }

    // TOP
    for(i = 1; p.x - i >= 0 && board[p.y][p.x - i] == -colour; i++){}
    if (p.x - i >= 0 && board[p.y][p.x - i] == colour){
        for(int j = 1; j < i; j++){
            board[p.y][p.x - j] = colour;
        }
    }
    // LEFT
    printf("%d, %d\n", p.x, p.y);
    for(i = 1; p.y - i >= 0 && board[p.y - i][p.x] == -colour; i++){}
    if (p.y - i >= 0 && board[p.y - i][p.x] == colour){
        for(int j = 1; j < i; j++){
            board[p.y - j][p.x] = colour;
        }
    }
    // RIGHT
    for(i = 1; p.y + i < 8 && board[p.y + i][p.x] == -colour; i++){}
    if (p.y + i < 8 && board[p.y + i][p.x] == colour){
        for(int j = 1; j < i; j++){
            board[p.y + j][p.x] = colour;
        }
    }
    // TOP RIGHT
    for(i = 1; p.x - i >= 0 && p.y + i < 8 && board[p.y + i][p.x - i] == -colour; i++){}
    if (p.x - i >= 0 && p.y + i < 8 && board[p.y + i][p.x - i] == colour){
        for(int j = 1; j < i; j++){
            board[p.y + j][p.x - j] = colour;
        }
    }
    // TOP LEFT
    for(i = 1; p.x - i >= 0 && p.y - i >= 0 && board[p.y - i][p.x - i] == -colour; i++){}
    if (p.x - i >= 0 && p.y - i >= 0 && board[p.y - i][p.x - i] == colour){
        for(int j = 1; j < i; j++){
            board[p.y - j][p.x - j] = colour;
        }
    }
    // BOT LEFT
    for(i = 1; p.x + i < 8 && p.y - i >= 0 && board[p.y - i][p.x + i] == -colour; i++){}
    if (p.x + i < 8 && p.y - i >= 0 && board[p.y - i][p.x + i] == colour){
        for(int j = 1; j < i; j++){
            board[p.y - j][p.x + j] = colour;
        }
    }
    // BOT RIGHT
    for(i = 1; p.x + i < 8 && p.y + i < 8 && board[p.y + i][p.x + i] == -colour; i++){}
    if (p.x + i < 8 && p.y + i < 8 && board[p.y + i][p.x + i] == colour){
        for(int j = 1; j < i; j++){
            board[p.y + j][p.x + j] = colour;
        }
    }
}

void printBoard(int** board){
    printf("/----------------\\");
    printf("\n");
    for(int i = 0; i < 8; i++){
        printf("|");
        for(int j = 0; j < 8; j++){
            if ( board[i][j] == 1 ){
                printf(" O");
            } else if ( board[i][j] == -1 ) {
                printf(" X");
            } else {
                printf("  ");
            }
        }
        printf("|\n");
    }
    printf("\\----------------/\n");
}


int pruning(int** board, int player) {
    int res;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if (board[i][j] == player){
                res += abs(4 - i) + abs(4-j);
            } else {
                res -= abs(4 - i) + abs(4-j);
            }
        }
    }
    
    return res;
}

valuation val_option(int v) {
    valuation r;
    r.v = v;
    return r;
}

valuation val_complet(int v, int x, int y) {
    valuation r;
    r.v = v;
    r.p = po(x, y);
    return r;
}

valuation alpha_beta(int** board, int alpha, int beta, int recursion_left, int player, int turn) {

    if (recursion_left == 0) {
        printBoard(board);
        valuation v = val_option(pruning(board, player));

        printf("Feuille atteinte calcul de l'heuristique : %d\n", v.v);

        return v;
    }
    
    pos* next_moves = possible_moves(board, player);
    valuation value;
    valuation nvalue;
    
    // On veut gagner
    if(player == turn){
        
        value.v = INT_MIN;
        for(int i = 0; next_moves[i].x != -1; i++ ){
            printf("NEXT MOVE: (%d, %d)\n", next_moves[i].x, next_moves[i].y);
            
            int** p = copy_board(board);
            place_stone(p, next_moves[i], player);
            
            nvalue = alpha_beta(p, alpha, beta, recursion_left - 1, player, -turn);

            free_board(p);

            if (value.v < nvalue.v){
                value.v = nvalue.v;
                value.p = next_moves[i];
            }
    // oublie pas de free
            alpha = max(alpha, value.v);
            // SOFT OU HARD ??
            if(value.v >= beta){
                break; // Beta cutoff
            }
        }
        printf("--> DEPTH : %d | Val maximise: %d\n", recursion_left, value.v);
        return value; // TODO: heuristic !!!!!

    // On veut perdre
    } else {

        value.v = INT_MAX;
        for (int i = 0; next_moves[i].x != -1; i++) {

            int** p = copy_board(board);
            place_stone(p, next_moves[i], player);

            nvalue = alpha_beta(p, alpha, beta, recursion_left - 1, player, turn);
            
            free_board(p);

            if (value.v > nvalue.v){
                value.v = nvalue.v;
                value.p = next_moves[i];
            }


            // oublie pas de free mdr

            beta = min(beta, value.v);

            if (value.v <= alpha) {
                break; // Alpha cutoff
            }

        }
        printf("--> DEPTH : %d | Val minimise: %d\n", recursion_left, value.v);
        return value;

    }

}
