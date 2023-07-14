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

/*---------------------------------------------------------*/
/*                        UTILITY                          */
/*---------------------------------------------------------*/
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

valuation val_option(float v) {
    valuation r;
    r.v = v;
    return r;
}

valuation val_complet(float v, int x, int y) {
    valuation r;
    r.v = v;
    r.p = po(x, y);
    return r;
}
/*---------------------------------------------------------*/
/*                   PRINT FUNCTIONS                       */
/*---------------------------------------------------------*/
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

/*---------------------------------------------------------*/
/*                    CREATE GAME                          */
/*---------------------------------------------------------*/


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
    //bool* allowed = (bool*)malloc(sizeof(bool) * BOARD_HEIGHT * BOARD_WIDTH);

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
                    break;
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
                    break;
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
                    break;
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
                    break;
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
                    break;
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
                    break;
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
                    break;
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
                    break;
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
                is_valid = false;

            }

        }
    }

    pos end_pos;
    end_pos.x = -1;
    end_pos.y = -1;
    results[current_index] = end_pos;

    return results;

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

int count_stones(int** board) {
    int c = 0;
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_HEIGHT; j++) {
            if (board[i][j] != 0) {
                c++;
            }
        }
    }
    return c;
}

float pruning_deep(int** board, int player, agent a) {

    // Nb of stone
    int stones = count_stones(board);

    // What part of the game
    int game_state;
    if (stones < 20) {
        game_state = 0; 
    } else if (stones < 45) {
        game_state = 1;
    } else {
        game_state = 2;
    }

    int score = 0;

    int board_multiplier = 1000;
    int mobility_multiplier;
    if (game_state == 0) {
        mobility_multiplier = 50;
    } else if (game_state == 1) {
        mobility_multiplier = 20;
    } else {
        mobility_multiplier = 100;
    }
    
    int stone_diff_multiplier;
    if (game_state == 0) {
        stone_diff_multiplier = 0;
    } else if (game_state == 1) {
        stone_diff_multiplier = 10;
    } else if (game_state == 2) {
        stone_diff_multiplier = 500;
    }
    

    int position_on_board[][8] = {
        {200 , -100, 100,  50,  50, 100, -100,  200},
        {-100, -200, -50, -50, -50, -50, -200, -100},
        {100 ,  -50, 100,   0,   0, 100,  -50,  100},
        {50  ,  -50,   0,   0,   0,   0,  -50,   50},
        {50  ,  -50,   0,   0,   0,   0,  -50,   50},
        {100 ,  -50, 100,   0,   0, 100,  -50,  100},
        {-100, -200, -50, -50, -50, -50, -200, -100},
        {200 , -100, 100,  50,  50, 100, -100,  200}
    };

    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_HEIGHT; j++) {

            if (board[i][j] == 0) {
                continue;
            } else if (board[i][j] == player) {
                score += board_multiplier * position_on_board[i][j];
            } else {
                score -= board_multiplier * position_on_board[i][j];
            }

        }
    }

    // Number of possible moves
    pos* pmove_player = possible_moves(board, player);
    pos* pmove_adversary = possible_moves(board, -player);
    
    int nb_move_score = 0;
    int i; int j;
    for (i = 0; pmove_player[i].x != -1; i++){}
    for (j = 0; pmove_adversary[j].x != -1; j++){}
    nb_move_score = (i - j);

    score += mobility_multiplier  * nb_move_score;
    
    // Difference of nb of stones
    int stone_diff_score = 0;
    for(int k = 0; k < 64; k++) {
        if(board[k/8][k%8] == player){
            stone_diff_score++;
        }else if(board[k/8][k%8] == -player){
            stone_diff_score--;
        }
    }
    score += stone_diff_multiplier * stone_diff_score;
    
    return score;
    

    // int W_size = 64;

    // float** tab = (float**)malloc(sizeof(float*)*W_size);

    // for(int i = 0; i < W_size; i++){
    //     tab[i] = (float*)malloc(sizeof(float));
    //     tab[i][0] = player*board[i / 8][i % 8]; // multiply by player so that its the 1 stones that need to be maximised
    // } 

    // // Board matrix column
    // matrix X = init_matrix(W_size, 1, tab);

    // // First layer
    // matrix temp1 = matrix_mult(a.W1, X);
    // matrix Z1 = matrix_add(temp1, a.b1);
    // matrix_sigmoid_self(Z1);

    // // Second layer
    // matrix Z2 = matrix_add(matrix_mult(a.W2, Z1), a.b2);
    // matrix_sigmoid_self(Z2);

    // // Thirs layer
    // matrix temp3 = matrix_mult(a.W3, Z2);
    // matrix Z3 = matrix_add(temp3, a.b3);
    // matrix_sigmoid_self(Z3);
    
    // free(tab);
    // return Z3.arr[0][0];
    
}

valuation alpha_beta(int** board, float alpha, float beta, int recursion_left, int player, int turn, agent a) {

    if (recursion_left == 0) {
        // printBoard(board);
        valuation v = val_option(pruning_deep(board, player, a));
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
        
        value.v = FLOAT_MINIMUM;
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

        value.v = FLOAT_MAXIMUM;
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

/*---------------------------------------------------------*/
/*             READING DATA + TOURNAMENT                   */
/*---------------------------------------------------------*/

agent create_agent() {

    agent a;

    int W_size = 64;

    a.W1 = null_matrix(W_size, W_size);
    a.W2 = null_matrix(W_size, W_size);
    a.W3 = null_matrix(1, W_size);
    a.b1 = null_matrix(W_size, 1);
    a.b2 = null_matrix(W_size, 1);
    a.b3 = null_matrix(1, 1);

    // printf("W1: ");
    for (int i = 0; i < W_size; i++) {
        for (int j = 0; j < W_size; j++) {
            scanf("%f", &a.W1.arr[i][j]);
        }
    }

    // printf("Received W1\n");

    //printf("W1 DONE");

    // printf("W2: ");
    for (int i = 0; i < W_size; i++) {
        for (int j = 0; j < W_size; j++) {
            scanf("%f", &a.W2.arr[i][j]);
        }
    }
    // printf("Received W2\n");

    // printf("W3: ");
    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < W_size; j++) {
            scanf("%f", &a.W3.arr[i][j]);
        }
    }
    // printf("Received W3\n");

    // printf("b1: ");
    for (int i = 0; i < W_size; i++) {
        for (int j = 0; j < 1; j++) {
            scanf("%f", &a.b1.arr[i][j]);
        }
    }
    // printf("Received b1\n");

    // printf("b2: ");
    for (int i = 0; i < W_size; i++) {
        for (int j = 0; j < 1; j++) {
            scanf("%f", &a.b2.arr[i][j]);
        }
    }
    // printf("Received b2\n");

    // printf("b3: ");
    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < 1; j++) {
            scanf("%f", &a.b3.arr[i][j]);
        }
    }
    // printf("Received b3\n");

    return a;

}

agent create_agent_from_file(FILE* f) {

    agent a;

    int W_size = 64;

    a.W1 = null_matrix(W_size, W_size);
    a.W2 = null_matrix(W_size, W_size);
    a.W3 = null_matrix(1, W_size);
    a.b1 = null_matrix(W_size, 1);
    a.b2 = null_matrix(W_size, 1);
    a.b3 = null_matrix(1, 1);

    // printf("W1: ");
    for (int i = 0; i < W_size; i++) {
        for (int j = 0; j < W_size; j++) {
            fscanf(f,"%f", &a.W1.arr[i][j]);
        }
    }

    // printf("Received W1\n");

    //printf("W1 DONE");

    // printf("W2: ");
    for (int i = 0; i < W_size; i++) {
        for (int j = 0; j < W_size; j++) {
            fscanf(f,"%f", &a.W2.arr[i][j]);
        }
    }
    // printf("Received W2\n");

    // printf("W3: ");
    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < W_size; j++) {
            fscanf(f,"%f", &a.W3.arr[i][j]);
        }
    }
    // printf("Received W3\n");

    // printf("b1: ");
    for (int i = 0; i < W_size; i++) {
        for (int j = 0; j < 1; j++) {
            fscanf(f,"%f", &a.b1.arr[i][j]);
        }
    }
    // printf("Received b1\n");

    // printf("b2: ");
    for (int i = 0; i < W_size; i++) {
        for (int j = 0; j < 1; j++) {
            fscanf(f,"%f", &a.b2.arr[i][j]);
        }
    }
    // printf("Received b2\n");

    // printf("b3: ");
    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < 1; j++) {
            fscanf(f,"%f", &a.b3.arr[i][j]);
        }
    }
    // printf("Received b3\n");

    return a;

}

agent* create_gen_from_file(FILE* f){
    agent* n = (agent*)malloc(sizeof(agent)*32);
    for(int i =0; i < 32; i++){
        n[i] = create_agent_from_file(f);
    }
    return n;
}

agent* create_gen_from_folder() {

    int agent_count = 32;

    agent* agent_array = (agent*)malloc(sizeof(agent) * agent_count);
    char file_location[40];
    FILE* f;

    for (int i = 0; i < agent_count; i++) {

        sprintf(file_location, "temp_agents/agent%d.txt", i);
        f = fopen(file_location, "r");

        agent_array[i] = create_agent_from_file(f);

        fclose(f);
    
    }

    return agent_array;

}

void do_tournament() {

    int agent_count = 32;

    agent* agent_array = create_gen_from_folder();

    int* values_arr = (int*)malloc(sizeof(int) * agent_count);
    for (int i = 0; i < agent_count; i++) {
        values_arr[i] = 0;
    }

    int temp_result;

    for (int i = 0; i < agent_count; i++) {
        for (int j = 0; j < agent_count; j++) {
            
            if (i == j) {
                continue;
            }

            temp_result = evaluate_two_agents(agent_array[i], agent_array[j]); // negative -> agent i won | positive -> agent j won

            if (temp_result < 0 ) {
                values_arr[i] += 100 - temp_result;
                values_arr[j] += temp_result;
            } else {
                values_arr[j] += 100 + temp_result;
                values_arr[i] += -temp_result;
            }

        }
    }

    printf("RESULT:");
    for (int i = 0; i < agent_count; i++) {
        if (i != 0) {
            printf(";");
        }
        printf("%d", values_arr[i]);
    }

}

int winner_of_board(int** board) { // postive -> white won | negative -> black won

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

    return (p1 >= p2) ? p1 : -p2;

}

int evaluate_two_agents(agent agent1, agent agent2) { // positive -> agent2 won  | negative -> agent1 won
    // agent 1 is black  |  agent 2 is white
    int** board = initialize_game();
    start_game(board);

    int MAX_PROFONDEUR = 1;

    int current_player = -1;
    int stones_placed = 4;

    for (int i = 0; stones_placed <= BOARD_HEIGHT * BOARD_WIDTH; i++) {

        // printf("Move #%d\n", stones_placed);
        // printBoard(board);

        pos* possible = possible_moves(board, current_player);
        if (possible[0].x == -1) {

            pos* other_poss = possible_moves(board, -current_player);
            if (other_poss[0].x == -1) {
                break;
            }

            current_player = -current_player;
            continue;

        }

        agent current_agent = current_player == -1 ? agent1 : agent2;
        valuation best_valuation = alpha_beta(board, FLOAT_MINIMUM, FLOAT_MAXIMUM, MAX_PROFONDEUR, current_player, current_player, current_agent);

        pos next_doable_move = best_valuation.p;

        place_stone(board, next_doable_move, current_player);
        
        current_player = -current_player;
        stones_placed++;

    }


    int result = winner_of_board(board); // positive -> agent2 won  | negative -> agent1 won

    return result;

}


/*---------------------------------------------------------*/
/*              PLAY WITH GAME FUNCTIONS                   */
/*---------------------------------------------------------*/

void play_game_2_players() {

    int** board = initialize_game();
    start_game(board);

    int stones_placed = 4;
    int current_player = -1;

    int move_x;
    int move_y;

    for (int i = 0; stones_placed <= BOARD_HEIGHT * BOARD_WIDTH; i++) {

        printf("---------- TURN %d ------------\n", i);

        pos* possible = possible_moves(board, current_player);
        if (possible[0].x == -1) {
            printf("No possible move\n");
            current_player = -current_player;
            continue;
        }

        printBoard(board);
        print_positions(possible);

        printf("Move of player %d:\n", current_player);
        printf("X: ");
        scanf("%d", &move_x);
        printf("Y: ");
        scanf("%d", &move_y);

        place_stone(board, po(move_x, move_y), current_player);

        current_player = -current_player;
        stones_placed++;

    }

}

void play_against_ai() {

    int** board = initialize_game();
    start_game(board);

    int stones_placed = 4;
    int current_player = -1;

    int ai_player = -1;

    int MAX_PROFONDEUR = 9;

    int move_x;
    int move_y;

    FILE* f = fopen("selected_agent.txt", "r");
    agent ai = create_agent_from_file(f);

    for (int i = 0; stones_placed <= BOARD_HEIGHT * BOARD_WIDTH; i++) {

        printf("---------- TURN %d ------------\n", i);

        pos* possible = possible_moves(board, current_player);
        if (possible[0].x == -1) {
            printf("No possible move\n");
            current_player = -current_player;

            pos* other_poss = possible_moves(board, -current_player);
            if (other_poss[0].x == -1) {
                break;
            }

            continue;
        }

        printBoard(board);
        print_positions(possible);

        if (current_player == -ai_player) {

            printf("Move of player %d:\n", current_player);
            printf("X: ");
            scanf("%d", &move_x);
            printf("Y: ");
            scanf("%d", &move_y);

            place_stone(board, po(move_x, move_y), current_player);

        } else {

            valuation best_valuation = alpha_beta(board, FLOAT_MINIMUM, FLOAT_MAXIMUM, MAX_PROFONDEUR, current_player, current_player, ai);
            pos next_doable_move = best_valuation.p;
            place_stone(board, next_doable_move, current_player);

        }

        current_player = -current_player;
        stones_placed++;

    }

}