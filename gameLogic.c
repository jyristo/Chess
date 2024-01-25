#include <stdio.h>
#include <stdlib.h>
#include "main.h"

void ask_move(char **board,int turn){
char start_pos[3];
char end_pos[3];
int correct_symbols=0;
int is_legal = 1;

        if(turn==0){
            printf("\nWhite's turn\n");
        }else{
            printf("\nBlack's turn\n");
        }

        while(is_legal == 1){

        printf("Enter piece start and end position ex. a2 a3\n");
        correct_symbols = scanf(" %[a-h] %[1-8]",&start_pos[0],&start_pos[1]);
        correct_symbols += scanf(" %[a-h] %[1-8]",&end_pos[0],&end_pos[1]);

        while (getchar() != '\n');
    
        while(correct_symbols != 4){
            printf("Invalid input, enter in format (a2 a3)\n");
            correct_symbols = scanf(" %[a-h] %[1-8]",&start_pos[0],&start_pos[1]);
            correct_symbols += scanf(" %[a-h] %[1-8]",&end_pos[0],&end_pos[1]);

            while (getchar() != '\n');
            }
        is_legal = check_move_legality(board,start_pos,end_pos,turn);
            if(is_legal==1){
                printf("INVALID MOVE\n");
            }
    }

}
int check_move_legality(char **board,char start[],char end[],int turn){

//Convert string coordinates to integers that match the board array
int start_col = (int)(start[0]-CHAR_TO_INT);
int start_row = (atoi(&(start[1])))-1;
int end_col = (int)(end[0]-CHAR_TO_INT);
int end_row = (atoi(&(end[1])))-1;
int is_legal=1;
printf(" %d %d %d %d - %c-",start_row,start_col,end_row,end_col,board[end_row][end_col]);

    if(board[start_row][start_col] == EMPTY_SQUARE){
        return 1;
    }

    //Check that there isn't our own piece in the landing square and we are not trying to move opponents piece
    //WHITE_PIECES(96) is used to check the color of the piece on the landing square(white pieces are lowercase ascii below 97)
    if(turn == 0 && (((board[end_row][end_col] < WHITE_PIECES) && (board[end_row][end_col] != EMPTY_SQUARE)) || (board[start_row][start_col] > WHITE_PIECES))){
        printf("FAIL 1;\n");
        return 1;
    }else if(turn == 1 && ((board[end_row][end_col] > WHITE_PIECES) || (board[start_row][start_col] < WHITE_PIECES ))){
       printf("FAIL 2;\n");
       return 1;
    }else{
        //Make the piece lowercase and initialize the enum variable
        char char_piece = (int)board[start_row][start_col];
        char_piece = (char_piece>= 'A' && char_piece<= 'Z') ? char_piece+('a'-'A'):char_piece;
        enum pieces piece = char_piece;

        switch (piece){
            case PAWN:
                //Check the square to move is free since pawns can't take moving forward
                is_legal = test_pawn_move(start_row,start_col,end_row,end_col,turn,board);

                //Check for pawn promotion;
                if((is_legal == 0 && end_row == 7) || (is_legal == 0 && end_row == 0)){
                    char queen_promotion = (turn == 0) ? 'Q': 'q';
                    board[start_row][start_col] = queen_promotion;
                }
                break;
            case ROOK:
                printf("\nROOK\n\n");
                break;
            case KNIGHT:
                printf("\nHORSE\n\n");
                break;
            case BISHOP:
                printf("\nBISHOP");
                break;
            case QUEEN:

                break;
            case KING:

                break;

        }

        if(is_legal ==0){
            board[end_row][end_col] = board[start_row][start_col];
            board[start_row][start_col]= EMPTY_SQUARE;
        }
        return is_legal;
    } 
    
}
int test_pawn_move(int start_row,int start_col,int end_row,int end_col,int turn,char **board){
int is_legal=1;
int is_empty=0;
int first_move;
int move_dir = (turn == 0) ? 1 : -1;
int col_change = abs(end_col - start_col);

is_empty = (board[end_row][end_col] == EMPTY_SQUARE) ? 1 : 0;

    if(is_empty){
        if((((start_row == (first_move = (turn == 0) ? 1 : 6)) && (end_row == (start_row + move_dir * 2))) || (end_row == start_row + (1 * move_dir))) && col_change == 0){
            is_legal = 0;
        }
    }else{
        if(col_change == 1 && (end_row == (start_row + (move_dir*1)))){
            is_legal = 0;
        }
    }

    return is_legal;
}
int test_rook_move(int start_row,int start_col,int end_row,int end_col,int turn){
return 0;
}