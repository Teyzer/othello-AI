#include <stdlib.h>
#include <stdio.h>
#include "othello.h"



int main() {
    
    int** board = initialize_game();
    start_game(board);
    printBoard(board);
    place_stone(board,po(2,3), -1);
    printBoard(board);
    place_stone(board,po(2,2), 1);
    printBoard(board);

    pos* possible = possible_moves(board, -1);
    print_positions(possible);

    for (int i = 0; i < 1; i++) {

        int player = (i % 2 == 0) ? -1 : 1;

        valuation v = alpha_beta(board, INT_MIN, INT_MAX, 1, player, player);
        printf("%d, (%d, %d)\n", v.v, v.p.x, v.p.y);
        place_stone(board, v.p, player);

    } 

    
    
    return 0;
}