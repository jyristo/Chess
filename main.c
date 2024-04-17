#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "main.h"

int main(void){
    // Initialize gamestate variables
    Gamestate* gamestate = (Gamestate*)malloc(sizeof(Gamestate));;
    gamestate->kingpos[WHITE] = 4;
    gamestate->kingpos[BLACK] = 60;
    gamestate->piece_count[WHITE] = 16;
    gamestate->piece_count[BLACK] = 16;
    gamestate->king_moves = 3;
    gamestate->rook_moves = 15;
    gamestate->game_over = 0;
    gamestate->turn = 1;

    // Initialize piece positions
    for(int i = 0; i < 16; i++){
        gamestate->pieces[WHITE][i] = i;
        gamestate->pieces[BLACK][i] = 48 + i;
    }

    // Create the board
    char** board = (char**)malloc(8 * sizeof(char*));
    if(board == NULL)
        return EXIT_FAILURE;
    
    for(int i = 0;i < 8;i++){
        board[i] = (char*)malloc(8*sizeof(char));
        if(board[i] == NULL){
            for(int j = 0;j < i;j++){
                free(board[j]);
            }
            return EXIT_FAILURE;
        }
    }
    printf("#######################################################################\n"
            "# Enter a move by writing the starting square and the end square\n"
            "# For example: h2 h4\n"
            "# After that the bot will make a random move\n"
            "#\n"
            "# The program will show you some possible key moves\n"
            "#######################################################################\n");

    init_board(board,gamestate);
    display(board);

    // Ask moves and display the board until game over
    while(!gamestate->game_over){
        gamestate->turn ^= 1;
        ask_move(board,gamestate);
        display(board);
    }
    display_winner(board,gamestate);


    for(int i=0;i<8;i++){
        free(board[i]);
    }
    board = NULL;
    free(gamestate);
    return 0;
}

