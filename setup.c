#include <stdio.h>
#include <stdlib.h>
#include "main.h"

void display(char **board){
    for(int i=7;i>=0;i--){
        printf("\n");
        for(int j=0;j<=7;j++){
           printf("%3c",board[i][j]);
        }
    }
}

void init_board(char** board){
    gameover =0;
    turn = 1;
    char init_vals[8][8] = {
       
        {'R','N','B','K','Q','B','N','R'},
        {'P','P','P','P','P','P','P','P'},
        {' ',' ',' ',' ',' ',' ',' ',' '},
        {' ',' ',' ',' ',' ',' ',' ',' '},
        {' ',' ',' ',' ',' ',' ',' ',' '},
        {' ',' ',' ',' ',' ',' ',' ',' '},
        {' ','p','p','p','p','p','p','p'},
        {' ','n','b','k','q','b','n','r'}

    };
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
           board[i][j]=init_vals[i][j];
        }
    }
}