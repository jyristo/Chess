#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "main.h"

void display(char **board){
    printf("\n");
    for(int i=7;i>=0;i--){
        printf("\n");
        for(int j=0;j<=7;j++){
           printf("%3c",board[i][j]);
        }
    }
}

void init_board(char** board){
    //Reinitialize game variables 
    gameover =0;
    turn = 0;
    king_moves = 0;
    move_count = 0;
    rook_moves = 0;
    
    char init_vals[8][8] = {  
        {'R',' ',' ',' ','K',' ',' ','R'},
        {'P','P','P',' ','p','P','P','P'},
        {' ',' ',' ',' ','n',' ',' ',' '},
        {' ',' ',' ',' ',' ',' ',' ',' '},
        {' ',' ',' ',' ',' ',' ',' ',' '},
        {' ',' ',' ',' ',' ',' ',' ',' '},
        {'p','p','p',' ','p','p','p','p'},
        {'r',' ',' ',' ','k',' ',' ','r'}
    };
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
           board[i][j]=init_vals[i][j];
        }
    }
}