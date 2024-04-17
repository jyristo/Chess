#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "main.h"

void display(char **board){
    printf("\n");
    char square = 254;
    int row = 8;
    for(int i=7;i>=0;i--){
        printf("\n %d|",row--);
        for(int j=0;j<=7;j++){
            // This is disgusting but i don't care i made it while i was furious (prints white squares to the board)
            if(((board[i][j] == EMPTY_SQUARE) && (((((i+1) % 2) != 1) && ((j+1) % 2 ) != 0)
            || (((i+1) % 2) == 1 && ((j+1) % 2 ) == 0)))){
                printf("%4c",square);
            }else
                printf("%4c",board[i][j]);
        }
    }
printf("\n  -----------------------------------\n      A   B   C   D   E   F   G   H\n");
}

void init_board(char** board, Gamestate* gamestate){
    //Initialize game variables 
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

    char init_vals[8][8] = {  
        {'R','N','B','Q','K','B','N','R'},
        {'P','P','P','P','P','P','P','P'},
        {' ',' ',' ',' ',' ',' ',' ',' '},
        {' ',' ',' ',' ',' ',' ',' ',' '},
        {' ',' ',' ',' ',' ',' ',' ',' '},
        {' ',' ',' ',' ',' ',' ',' ',' '},
        {'p','p','p','p','p','p','p','p'},
        {'r','n','b','q','k','b','n','r'}
    };
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
           board[i][j]=init_vals[i][j];
        }
    }
        printf("#######################################################################\n"
            "# Enter a move by writing the starting square and the end square\n"
            "# For example: h2 h4\n"
            "# After that the bot will make a random move\n"
            "#\n"
            "# The program will show you some possible key moves\n"
            "#######################################################################\n");

}
void display_winner(char** board,Gamestate* gamestate){
    int row = decode(gamestate->kingpos[gamestate->turn],ROW);
    int col = decode(gamestate->kingpos[gamestate->turn],COL);
    if(is_king_threatened(row,col,row,col,board,KING_MOVE,gamestate)){
        if(gamestate->turn == WHITE)
            printf("\n!!!!!!!!!!!BLACK WINS!!!!!!!!!!\n");
        else
            printf("\n!!!!!!!!!!!WHITE WINS!!!!!!!!!!\n");
    }else{
        printf("\nITS A STALEMATE\n");
    }
}