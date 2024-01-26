#include <stdio.h>
#include <stdlib.h>
#include "main.h"

int main(void){

    char **board = NULL;
    board = (char**)malloc(8 * sizeof(char*));

    if(board == NULL){
        printf("Memory allocating failed, exitting...");
        return 1;
    }else{
        printf("Memory allocated succesfully\n");
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
    printf("\n\nWelcome to chess\nEnter piece start and end position ex. a2 a3\n");
    turn = 0;

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

