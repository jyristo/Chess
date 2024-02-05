#include <stdio.h>
#include <stdlib.h>
#include "main.h"

int gameover = 0;
int turn = 1;
int king_moves = 0;
int move_count = 0;
int rook_moves = 0;

int main(void){
    printf("Moikka");
    //Allocate memory for the gameboard
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

    while(!gameover){
        ask_move(board,turn);
        display(board);
        turn ^= 1;
    }

    for(int i=0;i<8;i++){
        free(board[i]);
    }
    board = NULL;
    return 0;
}

