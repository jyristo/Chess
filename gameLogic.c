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
        move_count++;
    }

    printf("%d rook castling %d.\n",move_count,rook_moves);

    // Ask for a move until a legal move is entered 
    while(is_legal == 1){   
        correct_symbols = scanf(" %[a-h] %[1-8]",&start_pos[0],&start_pos[1]);
        correct_symbols += scanf(" %[a-h] %[1-8]",&end_pos[0],&end_pos[1]);

        // Clear input buffer
        while (getchar() != '\n');

        // Prompt the user until a valid input is reached
        while(correct_symbols != 4){  
            printf("Invalid input, enter in format (a2 a3)\n");
            correct_symbols = scanf(" %[a-h] %[1-8]",&start_pos[0],&start_pos[1]);
            correct_symbols += scanf(" %[a-h] %[1-8]",&end_pos[0],&end_pos[1]);
           
           // Clear input buffer
            while (getchar() != '\n');   
            }
        
        is_legal = check_move_legality(board,start_pos,end_pos,turn);
        if(is_legal==1){
            printf("INVALID MOVE\n");
        }
    }

}
int check_move_legality(char **board,char start[],char end[],int turn){

    // Convert string coordinates to integers that match the board array
    int start_col = (int)(start[0]-CHAR_TO_INT); // CHAR_TO_INT is used to convert the column value (a-h) to a char number then atoi for integer conversion
    int start_row = (atoi(&(start[1])))-1;
    int end_col = (int)(end[0]-CHAR_TO_INT);
    int end_row = (atoi(&(end[1])))-1;
    int is_legal=1;

    if(board[start_row][start_col] == EMPTY_SQUARE){
        return 1;
    }

    // Check that there isn't our own piece in the landing square and we are not trying to move opponents piece
    // WHITE_PIECES(96) is used to check the color of the piece on the landing square, white pieces are lowercase ascii (below 96)
    // For whites move we have to have an extra check for the empty squares
    if(turn == 0 && (((board[end_row][end_col] < WHITE_PIECES) && (board[end_row][end_col] != EMPTY_SQUARE)) || (board[start_row][start_col] > WHITE_PIECES))){
        return 1;
    // Same For black
    }else if(turn == 1 && ((board[end_row][end_col] > WHITE_PIECES) || (board[start_row][start_col] < WHITE_PIECES ))){
       return 1;
    }else{

        // Convert the piece we are moving to lowercase for the enum variable used in switch
        char char_piece = (int)board[start_row][start_col];
        char_piece = (char_piece>= 'A' && char_piece<= 'Z') ? char_piece+('a'-'A'):char_piece;
        enum pieces piece = char_piece;

        // Based on what piece we are moving we check if the move is legal
        switch (piece){
            case PAWN:
                is_legal = test_pawn_move(start_row,start_col,end_row,end_col,turn,board);

                // Check for pawn promotion;
                if((is_legal == 0 && end_row == 7) || (is_legal == 0 && end_row == 0)){
                    char queen_promotion = (turn == 0) ? 'Q': 'q';
                    board[start_row][start_col] = queen_promotion;
                }
                break;
            case ROOK:
                is_legal = test_rook_move(start_row,start_col,end_row,end_col,board);
                break;
            case KNIGHT:
                is_legal = test_knight_move(start_row,start_col,end_row,end_col);
                break;
            case BISHOP:
                is_legal = test_bishop_move(start_row,start_col,end_row,end_col,board);
                break;
            case QUEEN:
                is_legal = test_queen_move(start_row,start_col,end_row,end_col,board);
                break;
            case KING:
                is_legal = test_king_move(start_row,start_col,end_row,end_col,board);
                break;
        }

        if(is_legal == 0){
            board[end_row][end_col] = board[start_row][start_col];
            board[start_row][start_col ]= EMPTY_SQUARE;
        }
        // Finally return if the move is legal (0 legal, 1 illegal)
        return is_legal;
    } 
    
}
int test_pawn_move(int start_row,int start_col,int end_row,int end_col,int turn,char **board){
    int is_empty = 0;
    int first_move;
    int move_dir = (turn == 0) ? 1 : -1;
    int col_change = abs(end_col - start_col);

    is_empty = (board[end_row][end_col] == EMPTY_SQUARE) ? 1 : 0;

    // If the landing square is empty, check if pawn is in starting square, also check that pawn wont jump over a piece 
    if(is_empty){
        if(((((start_row == (first_move = (turn == 0) ? 1 : 6)) && (end_row == (start_row + move_dir * 2))) && board[end_row - (1*move_dir)][end_col] == EMPTY_SQUARE) ||
        (end_row == start_row + (1 * move_dir))) && col_change == 0){
            return 0;
        }
    }else{
        // If we are eating a piece, check that its done diagonally
        if(col_change == 1 && (end_row == (start_row + (move_dir*1)))){
            return 0;
        }
    }
    return 1;
}

int test_rook_move(int start_row,int start_col,int end_row,int end_col,char **board){
    int col_change = abs(end_col - start_col);
    int row_change = abs(end_row - start_row);
    int col_dir = (end_col > start_col) ? 1 : -1;
    int row_dir = (end_row > start_row) ? 1 : -1;
    int col_delta = (col_change == 0) ? 0 : 1;
    int row_delta = (row_change == 0) ? 0 : 1;

    // Check that we move only horizontally or vertically
   if((col_change == 0) || (row_change == 0)){
        //Check for obstruction
        for(int i=1;i<col_change + row_change;i++){
            if(board[start_row + (row_delta * row_dir * i) ][start_col + (col_delta * col_dir * i)] !=  EMPTY_SQUARE){
                return 1;
            }
        }
        //Update the rook_moves variable that is used to check for castling rights
        if((start_col == 0 || start_col == 7) && (start_row == 0 || start_row == 7) ){
            int rook_column = (start_col == 0) ? 0 : 1;
            int rook_color = (start_row == 0) ? 0 : 2;

            // rook_moves stores if the rooks have moved. 0001 white a-rook, 0010 white h-rook, 0100 black a-rook,1000 = black h-rook
            rook_moves |= (1U<<(rook_column + rook_color));
        }
        return 0;
   }
   return 1; 
}

int test_knight_move(int start_row,int start_col,int end_row,int end_col){
    int col_change = abs(end_col - start_col);
    int row_change = abs(end_row - start_row);

    // Knight can only move two suqares vertically and one horizontally or vice versa
    if((col_change == 1 && row_change == 2) || (col_change == 2 && row_change == 1)){
        return 0;
    }
    return 1;
}

int test_bishop_move(int start_row,int start_col,int end_row,int end_col,char **board){
    int col_change = abs(end_col - start_col);
    int row_change = abs(end_row - start_row);

    // If we aren't moving as much horizontally as vertically its not diagonal
    if(col_change == row_change){
        int col_dir = (end_col > start_col) ? 1 : -1;
        int row_dir = (end_row > start_row) ? 1 : -1;

        for(int i = 1;i< row_change;i++){
            if(board[start_row + (i * row_dir)][start_col + (i * col_dir)] != EMPTY_SQUARE){
                return 1;
            }
        }
        return 0;
    }
    return 1;
}

int test_queen_move(int start_row,int start_col,int end_row,int end_col,char **board){
    int col_change = abs(end_col - start_col);
    int row_change = abs(end_row - start_row);
    int col_dir = (end_col > start_col) ? 1 : -1;
    int row_dir = (end_row > start_row) ? 1 : -1;
    int col_delta = (col_change == 0) ? 0 : 1;
    int row_delta = (row_change == 0) ? 0 : 1;

    // For diagonal moves
    if(col_change == row_change){
        // Check for obsruction
        for(int i = 1;i< row_change;i++){
            if(board[start_row + (i * row_dir)][start_col + (i * col_dir)] != EMPTY_SQUARE){
                return 1;
            }
        }
        return 0;
    }
    // For rook moves
    else if((col_change == 0) || (row_change == 0)){
        //Check for obstruction
        for(int i=1;i<col_change + row_change;i++){
            if(board[start_row + (row_delta * row_dir * i) ][start_col + (col_delta * col_dir * i)] !=  EMPTY_SQUARE){
                return 1;
            }
        }
        return 0;
    }
    return 1;
}
int test_king_move(int start_row,int start_col,int end_row,int end_col,char **board){
    int col_change = abs(end_col - start_col);
    int row_change = abs(end_row - start_row);

    // For regular moves
    if((col_change <= 1) && (row_change <= 1)){
        // Update the king_moves variable used to check for castling rights
        king_moves |= (1U<<turn);
        return 0;
    }

    // Check if castling is allowed
    if(row_change == 0  && col_change == 2 && start_col == 4 ){
       return test_castling(start_row,start_col,end_row,end_col,board);
    }

    return 1;
}
int test_castling(int start_row,int start_col,int end_row,int end_col,char **board){
        int col_dir = (start_col < end_col) ? 1 : -1;
        int castling_distance = (end_col == 2) ? 4 : 3;
        int rook_col = (end_col == 2) ? 0 : 1;

        // Check if the king has moved
        if(king_moves & (1U<<turn)){
            return 1;
        }
        
        //Check if the rook has moved
        if(rook_moves & (1U<<(rook_col + turn*2))){
            return 1;
        }
        // Check for obstuction
        for(int i = 1; i < castling_distance;i++){
            if(board[start_row][start_col + (col_dir * i)] != EMPTY_SQUARE){
                return 1;
            }
        }
        // Check that king wont jump over threatened squares
    king_moves |= (1U<<turn);
    return 0;
}