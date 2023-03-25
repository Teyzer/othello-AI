#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "matrix.h"
#include "othello.h"

/*

CONVENTIONS
VIDE: 0
BLANC: 1
NOIR: -1

*/

float max(float a, float b) {
    return a > b ? a : b;
}

float min(float a, float b) {
    return a < b ? a : b;
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
                r.x = j;
                r.y = i;
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
    printf("/-----------------\\");
    printf("\n");
    for(int i = 0; i < 8; i++){
        printf("|");
        for(int j = 0; j < 8; j++){
            if ( board[i][j] == 1 ){
                printf(" O");
            } else if ( board[i][j] == -1 ) {
                printf(" X");
            } else if (board[i][j] == 0) {
                printf(" .");
            } else {
                printf(" L");
            }
        }
        printf(" |\n");
    }
    printf("\\-----------------/\n");
}


int pruning(int** board, int player, agent a) {

    int W_size = 64;

    float** tab = (float**)malloc(sizeof(float*)*W_size);
    
    for(int i = 0; i < W_size; i++){
        tab[i] = (float*)malloc(sizeof(float));
        tab[i][0] = board[i / 8][i % 8]; // comment ca mon beuf c une matrice colonisatrice 
    } // tous les tab[i][j] j != i sont pas définis là /!\ réel /!\ raciste
    // j'ai encore une fois rien dit, les colonisateurs sont en train de me corrompre

    matrix X = init_matrix(W_size, 1, tab);
    //matrix X = {.arr = tab, .height = 64, .width = 1};

    matrix temp1 = matrix_mult(a.W1, X);
    // printf("%d, %d, %d, %d", temp1.height, temp1.width, a.b1.height, a.b1.width);
    // print_matrix(temp1);
    // print_matrix(a.b1);
    // printf("Done first\n");
    matrix Z1 = matrix_add(temp1, a.b1);
    matrix_sigmoid_self(Z1);
    
    // printf("Done first\n");
    matrix Z2 = matrix_add(matrix_mult(a.W2, Z1), a.b2); // 
    matrix_sigmoid_self(Z2);

    // printf("Done first\n");
    matrix temp3 = matrix_mult(a.W3, Z2); // z2 c'est une colonne on est d'accord?
    // printf("Done second\n"); // oui c'était pas cette ligne là, c'était juste pour parler
    // printf("%d, %d\n", a.b3.height, a.b3.width); // mdr comment ça ca c normal le dernier noeud est tout seul il y a un seul bias
    // printf("%d, %d\n", temp3.height, temp3.width);  // wtf mais ca frero ligne * colonne cest de dim 1x1
    // printf("%d, %d, %d, %d\n", temp3.height, temp3.width, a.b3.width, a.b3.height);
    matrix Z3 = matrix_add(temp3, a.b3); // j'ai rien dis no comprendo c pas un bail de pointage en faisant le add et multiply en meme temps
    matrix_sigmoid_self(Z3);
    
    // printf("Done first\n");
    free(tab);

    // printf("Sait jamais");

    return Z3.arr[0][0];
    
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

valuation alpha_beta(int** board, float alpha, float beta, int recursion_left, int player, int turn, agent a) {

    if (recursion_left == 0) {
        // printBoard(board);
        valuation v = val_option(pruning(board, player, a));
        // printf("Feuille atteinte calcul de l'heuristique : %d\n", v.v);
        return v;
    }
    
    pos* next_moves = possible_moves(board, turn);
    // if(recursion_left == 1) {printf("FOR BOARD :\n");printBoard(board);printf("\nPositions for player %d : ", turn);print_positions(next_moves);printf("\n");}
    
    if (next_moves[0].x == -1) { // Pas de move possible
        return alpha_beta(board, alpha, beta, recursion_left - 1, player, -turn, a);
    }

    valuation value;
    valuation nvalue;
    
    // On veut gagner
    if(player == turn){
        
        value.v = INT_MIN;
        for(int i = 0; next_moves[i].x != -1; i++ ){
            // printf("NEXT MOVE: (%d, %d)\n", next_moves[i].x, next_moves[i].y);
            
            int** p = copy_board(board);
            place_stone(p, next_moves[i], turn);
            
            nvalue = alpha_beta(p, alpha, beta, recursion_left - 1, player, -turn, a);

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
        // printf("--> DEPTH : %d | Val maximise: %d\n", recursion_left, value.v);
        return value; // TODO: heuristic !!!!!

    // On veut perdre
    } else {

        value.v = INT_MAX;
        for (int i = 0; next_moves[i].x != -1; i++) {

            int** p = copy_board(board);
            place_stone(p, next_moves[i], turn);

            nvalue = alpha_beta(p, alpha, beta, recursion_left - 1, player, -turn, a);
            
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
        // printf("--> DEPTH : %d | Val minimise: %d\n", recursion_left, value.v);
        return value;

    }

}


/* ----------------- READING DATA ------------------ */

agent create_agent() {

    agent a;

    int W_size = 64;

    a.W1 = null_matrix(W_size, W_size);
    a.W2 = null_matrix(W_size, W_size);
    a.W3 = null_matrix(1, W_size);
    a.b1 = null_matrix(W_size, 1);
    a.b2 = null_matrix(W_size, 1);
    a.b3 = null_matrix(1, 1);

    printf("W1: ");
    for (int i = 0; i < W_size; i++) {
        for (int j = 0; j < W_size; j++) {
            scanf("%f", &a.W1.arr[i][j]);
        }
    }

    printf("Received W1\n");

    //printf("W1 DONE");

    printf("W2: ");
    for (int i = 0; i < W_size; i++) {
        for (int j = 0; j < W_size; j++) {
            scanf("%f", &a.W2.arr[i][j]);
        }
    }
    printf("Received W2\n");

    printf("W3: ");
    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < W_size; j++) {
            scanf("%f", &a.W3.arr[i][j]);
        }
    }
    printf("Received W3\n");

    printf("b1: ");
    for (int i = 0; i < W_size; i++) {
        for (int j = 0; j < 1; j++) {
            scanf("%f", &a.b1.arr[i][j]);
        }
    }
    printf("Received b1\n");

    printf("b2: ");
    for (int i = 0; i < W_size; i++) {
        for (int j = 0; j < 1; j++) {
            scanf("%f", &a.b2.arr[i][j]);
        }
    }
    printf("Received b2\n");

    printf("b3: ");
    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < 1; j++) {
            scanf("%f", &a.b3.arr[i][j]);
        }
    }
    printf("Received b3\n");

    return a;

}

int winner_of_board(int** board) {

    int p1 = 0, p2 = 0;

    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {

            if (board[i][j] == 1) {
                p1++;
            } else if (board[i][j] == -1) {
                p2++;
            } else {
                continue;
            }

        }
    }

    return p1 >= p2;

}

void evaluate_two_agents(agent agent1, agent agent2) {

    int** board = initialize_game();
    start_game(board);

    int MAX_PROFONDEUR = 5;

    int current_player = -1;
    int stones_placed = 4;

    for (int i = 0; stones_placed <= BOARD_HEIGHT * BOARD_WIDTH; i++) {

        printf("Move #%d\n", stones_placed);
        printBoard(board);

        pos* possible = possible_moves(board, current_player);
        if (possible[0].x == -1) {

            pos* other_poss = possible_moves(board, current_player);
            if (other_poss[0].x == -1) {
                break;
            }

            current_player = -current_player;
            continue;

        }

        agent current_agent = current_player == -1 ? agent1 : agent2;
        valuation best_valuation = alpha_beta(board, INT_MIN, INT_MAX, MAX_PROFONDEUR, current_player, current_player, agent1);

        pos next_doable_move = best_valuation.p;

        place_stone(board, next_doable_move, current_player);
        
        current_player = -current_player;
        stones_placed++;

    }   

    bool result = winner_of_board(board);

    if (result) {
        printf("RESULT:1\n");
    } else {
        printf("RESULT:2\n");
    }

}