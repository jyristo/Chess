#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "main.h"

int game_over = 0;
int turn = 0;
int king_moves = 0;
int move_count = 1;
int rook_moves = 0;
Move last_move = {0,0,0,0, ' '};
Piece white_king = {.row = 0,.col = 4};
Piece black_king = {.row = 7,.col = 4};
int check_mate = 0;

int main(void){
    // Create the board
    char **board = NULL;
    board = (char**)malloc(8 * sizeof(char*));
    if(board == NULL){
        printf("Memory allocating failed, exitting...");
        return 1;
    }

        for(int i=0;i<8;i++){
            board[i] = (char*) malloc(8*sizeof(char));
            if(board[i] == NULL){
                printf("Memory allocating failed\n");
                for(int j=0;j<i;j++){
                    free(board[j]);
                }
            }
        }
    init_board(board);
    display(board);

    // Superloop: Ask moves and display the board until game over
    while(!game_over){
        ask_move(board);
        display(board);
        turn ^= 1;
    }
    display_winner();

    // Destroy the board
    for(int i=0;i<8;i++){
        free(board[i]);
    }
    board = NULL;
    return 0;
}

