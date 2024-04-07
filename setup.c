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
    game_over = 0;
    turn = 0;
    king_moves = 3;
    move_count = 1;
    rook_moves = 15;
    
    char init_vals[8][8] = {  
        {'R',' ',' ',' ','K',' ',' ','R'},
        {'P','P','P','P','P','P','P','P'},
        {' ',' ',' ',' ',' ','n',' ',' '},
        {' ',' ',' ',' ',' ',' ',' ',' '},
        {' ',' ',' ',' ',' ','b',' ',' '},
        {' ',' ',' ',' ','q',' ',' ',' '},
        {'p','p','p','p','p','p','p','p'},
        {'r','n',' ','q','k',' ',' ','r'}
    };
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
           board[i][j]=init_vals[i][j];
        }
    }
}
void display_winner(){
    if(turn == 0){
        printf("\nCHECKMATE---WHITE WINS\n");
    }else{
        printf("\nCHECKMATE---BLACK WINS\n");
    }
}