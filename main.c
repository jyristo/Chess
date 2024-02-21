#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "main.h"

int gameover = 0;
int turn = 1;
int king_moves = 0;
int move_count = 0;
int rook_moves = 0;
int white_king_pos = 4;
int black_king_pos = 60;
Move last_move = {0,0,0,0, ' '};
Pieces black_pieces = {}
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
    while(!gameover){
        ask_move(board,turn);
        display(board);
        turn ^= 1;
    }

    // Destroy the board
    for(int i=0;i<8;i++){
        free(board[i]);
    }
    board = NULL;
    return 0;
}

