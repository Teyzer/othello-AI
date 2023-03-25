#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"
#include "othello.h"


int main() {
    
    int** board = initialize_game();
    start_game(board);

    // matrix f = null_matrix(1, 64);
    // matrix g = null_matrix(64, 1);

    // matrix h = matrix_mult(f, g);
    // h = matrix_add(h, h);

    // print_matrix(h);

    agent a1 = create_agent();
    agent a2 = create_agent();

    
    // printf("WTF PRINT PLEASE\n");


    evaluate_two_agents(a1, a2);

    // matrix m = identity_matrix(10);
    // print_matrix(m);

    // agent a = create_agent();

    // int** b = initialize_game();
    // start_game(b);
    // place_stone(b, po(2, 3), -1);
    // place_stone(b, po(1, 3), 1);
    // printBoard(b);

    return 0;
}