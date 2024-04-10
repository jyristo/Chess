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
    turn = 1;
    king_moves = 3;
    move_number = 1;
    rook_moves = 15;
    
    char init_vals[8][8] = {  
        {'R','N','B','Q','K','B','N','R'},
        {'P','P','P','P','P','P','P','P'},
        {' ',' ',' ',' ',' ',' ',' ',' '},
        {' ',' ',' ',' ',' ',' ',' ',' '},
        {' ',' ',' ',' ',' ',' ','P',' '},
        {' ',' ',' ',' ',' ',' ',' ',' '},
        {'p','p','p','p','p','p','p','p'},
        {'r','n','b','q','k','b','n','r'}
    };
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
           board[i][j]=init_vals[i][j];
        }
    }
}
void display_winner(char** board){
    Piece* king_ptr = (turn == WHITE) ? &white_king : &black_king;
    int row = king_ptr->row;
    int col = king_ptr->col;
    if(is_king_threatened(row,col,row,col,board,1)){
        if(turn == WHITE)
            printf("\n!!!!!!!!!!!BLACK WINS!!!!!!!!!!\n");
        else
            printf("\n!!!!!!!!!!!WHITE WINS!!!!!!!!!!\n");
    }else{
        printf("\nITS A STALEMATE YOU ALL SUCK ASS FUCK YOU EAT SHIT\n");
    }
}