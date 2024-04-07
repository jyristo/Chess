#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "main.h"

void ask_move(char **board){
    char start_pos[3];
    char end_pos[3];
    int correct_symbols = 0;
    int is_legal = 0;

    // See if player is in checkmate
    test_for_mate(board);

    if(game_over){
        return;
    }
    #ifdef DEBUG
        printf("\n\nWhite %d %d\nBlack %d %d\nrook moves: %d\nking moves: %d\n",white_king.row,white_king.col,black_king.row,black_king.col,rook_moves,king_moves);
        printf("Last move was %d %d to %d %d\n",last_move.start_col,last_move.start_row,last_move.end_col,last_move.end_row);
    #endif
    if(turn == WHITE){
        printf("\n%d. White's turn\n",move_count);
    }else{
        printf("\n%d. Black's turn\n",move_count);
        move_count++;
    }

    // Ask for a move until a legal move is entered 
    while(is_legal != 1){   
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

        if(is_legal != 1){
            printf("INVALID MOVE\n");
        }
    }
}
int check_move_legality(char **board,char start[],char end[],int turn){
    // Convert string coordinates to integers that match the board array
    // CHAR_TO_INT is used to convert the column value (a-h) to a char number, then atoi for integer conversion
    int start_col = (int)(start[0]-CHAR_TO_INT); 
    int start_row = atoi(&(start[1]))-1;
    int end_col = (int)(end[0]-CHAR_TO_INT);
    int end_row = atoi(&(end[1]))-1;
    int is_legal = 0;
    int in_check = 0;
    // Check there is a piece on the starting square
    if(board[start_row][start_col] == EMPTY_SQUARE){
        return 0;
    }
    // Check that there isn't our own piece in the landing square and we are not trying to move opponents piece
    // WHITE_PIECES(96) is used to check the color of the piece on the landing square, white pieces are uppercase ascii (below 96)
    // For whites move we have to have an extra check for the empty squares
    if(turn == WHITE && (((board[end_row][end_col] < WHITE_PIECES) && (board[end_row][end_col] != EMPTY_SQUARE)) || (board[start_row][start_col] > WHITE_PIECES))){
        return 0;
    // Same For black
    }else if(turn == BLACK && ((board[end_row][end_col] > WHITE_PIECES) || (board[start_row][start_col] < WHITE_PIECES ))){
       return 0;
    }
    // Convert the piece we are moving to lowercase for the enum variable used in switch
    char char_piece = board[start_row][start_col];
    char_piece = (char_piece >= 'A' && char_piece <= 'Z') ? char_piece + ('a'-'A'):char_piece;
    enum pieces piece = char_piece;

    // Based on what piece we are moving check if the move is legal
    switch (piece){
        case PAWN:
            is_legal = test_pawn_move(start_row,start_col,end_row,end_col,board);
            // Check for pawn promotion;
            if(is_legal == 1 && (end_row == 7 || end_row == 0)){
                char queen_promotion = (turn == WHITE) ? 'Q': 'q';

                // The pawn is first turned in to a queen since the actual gameboard updating happens in finalize_move()
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
    printf("\nis_legal: %d\n",is_legal);
    // Check that the king isn't threatend resulting in the move
    in_check = test_for_checks(start_row, start_col, end_row, end_col,board);
    printf("is_legal: %d\n",is_legal);

    // Update the board, last move variable and king position variable
    if(is_legal  && !in_check){
        printf("\nThe move is legal\n");
        finalize_move(start_row, start_col, end_row, end_col,board);
    }else{
        is_legal = 0;
    }
    // Finally return if the move is legal 
    return is_legal;
}
int get_square_status(char** board,int row, int col){
    int piece = board[row][col];
    if((turn == WHITE && piece > WHITE_PIECES) || (turn == BLACK && piece < WHITE_PIECES && piece != EMPTY_SQUARE))
        return ENEMY_PIECE;
    else if(piece == EMPTY_SQUARE)
        return EMPTY_SQUARE;
    else
        return FRIENDLY_PIECE;
}
int vertical_ray(int row,int col,int new_row,int new_col,char** board){
    int dir = (new_row > row) ? 1 : -1;
    row += dir;

    if(col != new_col)
        return 0;

    while(row != new_row && row < 8 && row  >= 0){
        if(board[row][col] != EMPTY_SQUARE)
            return 0;
        row += dir;
    }
    if(row == new_row)
        return 1;
    return 0;
}
int horizontal_ray(int row,int col,int new_row,int new_col,char** board){
    int dir = (new_col > col) ? 1 : -1;
    col += dir;
    // Early exit if not in same column
    if(row != new_row)
        return 0;

    while(col != new_col && col < 8 && col >= 0){
        if(board[row][col] != EMPTY_SQUARE)
            return 0;
        col += dir;
    }
    if(col == new_col)
        return 1;
    return 0;
}
int test_rook_threat(int row,int col,int new_row,int new_col,char** board){
    return vertical_ray(row,col,new_row,new_col,board) || horizontal_ray(row,col,new_row,new_col,board);
}
int test_bishop_threat(int row,int col,int new_row,int new_col,char** board){
    return diagonal_ray(row,col,new_row,new_col,board);
}
int test_queen_threat(int row,int col,int new_row,int new_col,char** board){
    int i = test_bishop_threat(row,col,new_row,new_col,board);
    int j = test_rook_threat(row,col,new_row,new_col,board);
    //printf("bishop: %d rook: %d tested with %d %d %d %d\n",i,j,row,col,new_row,new_col);
    return i || j;
}
int test_pawn_threat(int row,int col,int new_row,int new_col,char** board){
    int dir = (turn == WHITE) ? -1 : 1;
    if(new_row == row + dir && (new_col == col + 1 || new_col == col - 1))
        return 1;
    return 0;
}
int test_knight_threat(int row,int col,int king_row,int king_col,char** board){
    int row_change = abs(king_row - row);
    int col_change = abs(king_col - col);

    if((row_change == 1 && col_change == 2) || (row_change == 2 && col_change == 1))
        return 1;
    return 0;
}
int diagonal_ray(int row,int col,int new_row,int new_col,char** board){
    int row_dir = (new_row > row) ? 1 : -1;
    int col_dir = (new_col > col) ? 1 : -1;
    col += col_dir;
    row += row_dir;
    while(col != new_col && row != new_row && col < 8 && col >= 0 && row < 8 && row  >= 0 ){
        if(board[row][col] != EMPTY_SQUARE){
            return 0;
        }
        col += col_dir;
        row += row_dir;
    }
    if(row == new_row && col == new_col)
        return 1;
    return 0;
}
void make_move(int row,int col,int new_row,int new_col,char** board){

}
int test_king_safety(int row,int col,int new_row,int new_col,char** board){
    int king_row = (turn == WHITE) ? white_king.row : black_king.row;
    int king_col = (turn == WHITE) ? white_king.col : black_king.col;
    int threat = 0;
    char previous_piece = board[new_row][new_col];
    board[new_row][new_col] = board[row][col];
    board[row][col] = EMPTY_SQUARE;

    for(int i = 0; i < 8;i++){
        for(int j = 0;j < 8;j++){
            char piece = board[i][j];
            int square = get_square_status(board,i,j);
            if(square == ENEMY_PIECE){
                if(piece == 'R' || piece == 'r')
                    threat = test_rook_threat(i,j,king_row,king_col,board);
                else if(piece == 'B' || piece == 'b')
                    threat = test_bishop_threat(i,j,king_row,king_col,board);
                else if(piece == 'Q' || piece == 'q')
                    threat = test_queen_threat(i,j,king_row,king_col,board);
                else if(piece == 'P' || piece == 'p')
                    threat = test_pawn_threat(i,j,king_row,king_col,board);
                else if(piece == 'N' || piece == 'n')
                    threat = test_knight_threat(i,j,king_row,king_col,board);
                else{
                    // The piece is king so do nothing
                }
            }
            if(threat){
                printf("---THREAT FOUND AT %d %d by piece %c theatens %d %d---\n",i,j,piece,king_row,king_col);
                board[row][col] = board[new_row][new_col];
                board[new_row][new_col] = previous_piece;
                return threat;
            }
        }
    }
    board[row][col] = board[new_row][new_col];
    board[new_row][new_col] = previous_piece;
    return threat;
}
void compute_moves(char** board,int* moves,int* move_count){
    for(int i = 0;i < 8;i++){
        for(int j = 0;j < 8;j++){
            int current_piece = board[i][j];
            if((turn == WHITE && current_piece < WHITE_PIECES && current_piece != EMPTY_SQUARE ) || (turn == BLACK && current_piece > WHITE_PIECES)){
                current_piece = (current_piece >= 'A' && current_piece <= 'Z') ? current_piece + ('a'-'A'):current_piece;
                enum pieces piece = current_piece;
                switch (piece){
                case PAWN:
                    compute_pawn_moves(i,j,board,moves,move_count);
                    break;
                case ROOK:
                    //compute_rook_moves(i,j,board,moves,move_count);
                    break;
                case KNIGHT:
                   // compute_knight_moves(i,j,board,moves,move_count);
                    break;
                case BISHOP:
                    //compute_bishop_moves(i,j,board,moves,move_count);
                    break;
                case QUEEN:
                    //compute_queen_moves(i,j,board,moves,move_count);
                    break;
                case KING:
                    compute_king_moves(i,j,board,moves,move_count);
                    break;
                }
            }
        }
    }
}
void compute_pawn_moves(int row,int col,char** board,int* moves, int* move_count){
    compute_forward_moves(row,col,board,moves,move_count);
    compute_capture_moves(row,col,board,moves,move_count);
    compute_en_passant_moves(row,col,board,moves,move_count);
}
void compute_forward_moves(int row,int col,char** board,int* moves,int* move_count){
    int first_move = ((turn == WHITE && row == 1) || (turn == BLACK && row == 6)) ? 1 : 0;
    int move_dir = (turn == WHITE) ? 1 : -1;
    // Single row moves
    if(board[row + move_dir][col] == EMPTY_SQUARE  && !test_king_safety(row,col,row + move_dir,col,board)){
        add_move(row,col,row + move_dir,col,moves,move_count);
     // Two row moves
        if(first_move && board[row + move_dir*2][col] == EMPTY_SQUARE && !test_king_safety(row,col,row + move_dir*2,col,board))
        add_move(row,col,row + move_dir*2,col,moves,move_count);
    }
}
void compute_capture_moves(int row,int col,char** board,int* moves,int* move_count){
    int move_dir = (turn == WHITE) ? 1 : -1;
    int left_col = col - 1;
    int right_col = col + 1;
    if(left_col < 8 && left_col >=0){
        int left_piece = get_square_status(board,row + move_dir,left_col);
        if(left_piece == ENEMY_PIECE && !test_king_safety(row,col,row + move_dir,left_col,board)){
            printf("Added move %d %d %d %d\n",row,col,row + move_dir,left_col);
            add_move(row,col,row + move_dir,left_col,moves,move_count);

        }
    }if(right_col < 8 && right_col >= 0){
        int right_piece = get_square_status(board,row + move_dir,right_col);
        if((right_piece == ENEMY_PIECE && !test_king_safety(row,col,row + move_dir,right_col,board))){
            printf("Added move %d %d %d %d\n",row,col,row + move_dir,right_col);
            add_move(row,col,row + move_dir,right_col,moves,move_count);
        }
    }
}
void add_move(int start_row,int start_col,int end_row,int end_col,int* moves,int* move_count){
    int move = start_row * 8 + start_col;
    moves[*move_count] = move | ((end_row * 8 + end_col)<<6);
    (*move_count)++;
}
void compute_en_passant_moves(int row,int col,char** board,int* moves,int* move_count){
    if(!(last_move.piece == 'p' || last_move.piece == 'P') || last_move.end_row != row ||
         abs(last_move.start_row - last_move.end_row) != 2 || abs(last_move.end_col - col) != 1 ){
        return;
   }
    int dir = (turn == WHITE) ? 1 : -1;
    add_move(row,col,row + dir,last_move.start_col,moves,move_count);
}
void compute_knight_moves(int row,int col,char** board,int* moves,int* move_count){
    for(int dir1 = -1; dir1 <= 1;dir1 +=2){
        for(int dir2 = -1; dir2 <= 1;dir2 +=2){
            int new_row = row + 2 * dir1;
            int new_col = col + 1 * dir2;
            if(new_row >= 0 && new_row < 8 && new_col >= 0 && new_col < 8){
                int free_square = get_square_status(board,new_row,new_col);
                if(free_square != FRIENDLY_PIECE)
                    add_move(row,col,new_row,new_col,moves,move_count);
            }
            new_row =  row + (1*dir1);
            new_col = col + (2*dir2);
            if(new_row >= 0 && new_row < 8 && new_col >= 0 && new_col < 8){
                int free_square = get_square_status(board,new_row,new_col);
                if(free_square != FRIENDLY_PIECE)
                    add_move(row,col,new_row,new_col,moves,move_count);
            }
        }
    }
}
void compute_rook_moves(int row,int col,char** board,int* moves,int* move_count){
    compute_horizontal_moves(row,col,board,moves,move_count);
    compute_vertical_moves(row,col,board,moves,move_count);
}
void compute_horizontal_moves(int row,int col,char** board,int* moves,int* move_count){
    int new_col = col;
    for(int dir = -1; dir <= 1;dir += 2){
        int current_square = EMPTY_SQUARE;
        new_col = col + dir;
        while(new_col < 8 && new_col >= 0 && current_square == EMPTY_SQUARE){
            current_square = get_square_status(board,row,new_col);
            if(current_square == EMPTY_SQUARE)
                add_move(row,col,row,new_col,moves,move_count);
            else if(current_square == ENEMY_PIECE)
                add_move(row,col,row,new_col,moves,move_count);
            new_col += dir;
        }
    }
}
void compute_vertical_moves(int row,int col,char** board,int* moves,int* move_count){
    int new_row = row;
    for(int dir = -1; dir <= 1;dir += 2){
        int current_square = EMPTY_SQUARE;
        new_row = row + dir;
        while(new_row < 8 && new_row >= 0 && current_square == EMPTY_SQUARE){
            current_square = get_square_status(board,new_row,col);
            if(current_square == EMPTY_SQUARE || current_square == ENEMY_PIECE)
                add_move(row,col,new_row,col,moves,move_count);
            new_row += dir;
        }
    }
}
void compute_diagonal_moves(int row,int col,char** board,int* moves,int* move_count){
    for(int dir1 = -1;dir1 <= 1; dir1 += 2){
        for(int dir2 = -1;dir2 <= 1;dir2 += 2){
            int current_square = EMPTY_SQUARE;
            int new_row = row + dir1;
            int new_col = col + dir2;
            while(new_row < 8 && new_row >= 0 && new_col < 8 && new_col >= 0 && current_square == EMPTY_SQUARE){
                current_square = get_square_status(board,new_row,new_col);
                if(current_square == EMPTY_SQUARE || current_square == ENEMY_PIECE){
                    add_move(row,col,new_row,new_col,moves,move_count);
                }   
                new_row += dir1;
                new_col += dir2;
            }
        }
    }
}
void compute_king_moves(int row,int col,char** board,int* moves,int* move_count){
    for(int dir1 = -1; dir1 <= 1;dir1++){
        for(int dir2 = -1;dir2 <= 1;dir2++){
            int current_square = EMPTY_SQUARE;
            int new_row = row + dir1;
            int new_col = col + dir2;
            if(new_row < 8 && new_row >= 0 && new_col < 8 && new_col >= 0  && !(new_row == row && new_col == col)){
                current_square = get_square_status(board,new_row,new_col);
                if(current_square == EMPTY_SQUARE || current_square == ENEMY_PIECE)
                    add_move(row,col,new_row,new_col,moves,move_count);
            }
        }
    }
    if(king_moves & (1U<<turn))
        compute_castling(row,col,board,moves,move_count);
}
void compute_castling(int row,int col,char** board,int* moves,int* move_count){
    int current_square = EMPTY_SQUARE;
    int castling_rights = 1;
    if(rook_moves & 1U<<turn){
        for(int new_col = col - 1; new_col >= 1;new_col--){
            current_square = get_square_status(board,row,new_col);
            if(current_square != EMPTY_SQUARE)
                castling_rights = 0;
        }
        if(castling_rights)
            add_move(row,col,row,col - 2,moves,move_count);
    }
    castling_rights = 1;
    // Test for other direction
    if(rook_moves & 1U<<(turn + 1)){
    for(int new_col = col + 1; new_col <= 5;new_col++){
            current_square = get_square_status(board,row,new_col);
            if(current_square != EMPTY_SQUARE)
                castling_rights = 0;
        }
        if(castling_rights)
            add_move(row,col,row,col + 2,moves,move_count);
    }
}
void compute_bishop_moves(int row,int col,char** board,int* moves,int* move_count){
    compute_diagonal_moves(row,col,board,moves,move_count);
}
void compute_queen_moves(int row,int col,char** board,int* moves,int* move_count){
    compute_rook_moves(row,col,board,moves,move_count);
    compute_bishop_moves(row,col,board,moves,move_count);
}

int test_for_checks(int start_row,int start_col,int end_row,int end_col,char** board){
    // Choose which king we are interested in
    Piece *king_ptr = (turn == WHITE) ? &white_king : &black_king;
    int check = 0;

    // Store the king position in a temporary variable
    int king_position_row = king_ptr->row;
    int king_position_col = king_ptr->col;

    // Create a temporary board to test the move
    char **temp_board = NULL;
    temp_board = (char**)malloc(8 * sizeof(char*));
      if(temp_board == NULL){
        printf("Memory allocating failed, exitting...");
        return 5;
    }
        for(int i=0;i<8;i++){
           temp_board[i] = (char*) malloc(8*sizeof(char));
            if(board[i] == NULL){
                printf("Memory allocating failed\n");
                for(int j=0;j<i;j++){
                    free(board[j]);
                }
            }
        }
    // Copy main board to temp board
    for(int i = 0;i<8;i++){
        for(int j = 0;j<8; j++){
            temp_board[i][j] = board[i][j];
        }
    }

    // Since king position is updated in the next function, if we are testing a king move...
    // We have to update the temp variable, otherwise we reference the previous king position
    if(temp_board[start_row][start_col] == 'k' || temp_board[start_row][start_col] == 'K'){
        king_position_col = end_col;
        king_position_row = end_row;
    }

    // Make the move in the temp board
    temp_board[end_row][end_col] = temp_board[start_row][start_col];
    temp_board[start_row][start_col] = EMPTY_SQUARE;
    // Check if any enemy piece can attack the file the king is in
    for(int i = 0;i < 8;i++){
        for(int j = 0;j < 8;j++){
            if((turn == WHITE && temp_board[i][j] > WHITE_PIECES) || (turn == BLACK && temp_board[i][j] < WHITE_PIECES && temp_board[i][j] != EMPTY_SQUARE)){
                check = test_for_threat(i,j,king_position_row,king_position_col,temp_board);
            
                #ifdef DEBUG
                    printf("testing piece %c can threat %d position %d %d\n",temp_board[i][j],check,king_position_row,king_position_col);
                #endif

                if(check == 1){
                    // Destroy temp board
                    for(int i=0;i<8;i++){
                        free(temp_board[i]);  
                    }
                    return 1;
                } 
            }
        }
    }
     // Destroy temp board
    for(int i=0;i<8;i++){
        free(temp_board[i]);
    }
    return 0;
}
void finalize_move(int start_row,int start_col,int end_row,int end_col,char **board){
    // Update the game board
    board[end_row][end_col] = board[start_row][start_col];
    board[start_row][start_col ]= EMPTY_SQUARE;
    // Update last move variable (Used for en passant)
    last_move.start_col = start_col;
    last_move.start_row = start_row;
    last_move.end_col = end_col;
    last_move.end_row = end_row;
    last_move.piece = board[end_row][end_col];

    // Update king position
    if(board[end_row][end_col] == 'K'){
        white_king.row = end_row;
        white_king.col = end_col;
        king_moves  &=~(1U<<0);

    }else if(board[end_row][end_col] == 'k'){
        black_king.row = end_row;
        black_king.col = end_col;
        king_moves &=~(1U<<1);

    }
}
int test_pawn_move(int start_row,int start_col,int end_row,int end_col,char **board){
    int first_move = (board[start_row][start_col] == 'P') ? 1 : 6;
    int move_dir = (board[start_row][start_col] == 'P') ? 1 : -1;
    int col_change = abs(end_col - start_col);
    int landing_square_empty = (board[end_row][end_col] == EMPTY_SQUARE) ? 1 : 0;

    // If the landing square is empty and we are staying in the same column, check if pawn is in starting square
    // also check that pawn wont jump over a piece 
    if(landing_square_empty){
        if(col_change == 0){
            if(((start_row == first_move) && (end_row == start_row + move_dir * 2)) && (board[start_row + move_dir][start_col] == EMPTY_SQUARE)){
                return 1;
            }
            // Else check that we are only moving one square
            if(end_row == start_row + move_dir){
                return 1;
            }    
        return 0;
        // Check for en passant
        }else if(col_change == 1 && end_col == last_move.end_col){
            if((turn == 0 && last_move.piece == 'p' )|| (turn == 1 && last_move.piece == 'P')){
                if(last_move.end_row == start_row && (last_move.end_row == (last_move.start_row + 2) || last_move.end_row == (last_move.start_row - 2) )){
                    board[last_move.end_row][last_move.end_col] = ' ';
                    return 1;
                }
            }
        }
        return 0;
    }
    // If we are eating a piece, check that its done diagonally
    else if(col_change == 1 && end_row == (start_row + move_dir)){
            return 1;
    }
    return 0;
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
                return 0;
            }
        }
        // Update the rook_moves variable that is used to check for castling rights
        // 0001 white a-rook, 0010 white h-rook, 0100 black a-rook,1000 = black h-rook 
        if((start_col == 0 || start_col == 7) && (start_row == 0 || start_row == 7)){
            int rook_column = (start_col == 0) ? 0 : 1;
            int rook_color = (start_row == 0) ? 0 : 2;

            rook_moves &=~(1U<<(rook_column + rook_color));
        }
        return 1;
   }
   return 0; 
}
int test_knight_move(int start_row,int start_col,int end_row,int end_col){
    int col_change = abs(end_col - start_col);
    int row_change = abs(end_row - start_row);

    // Knight can only move two suqares vertically and one horizontally or vice versa
    if((col_change == 1 && row_change == 2) || (col_change == 2 && row_change == 1)){
        return 1;
    }
    return 0;
}
int test_bishop_move(int start_row,int start_col,int end_row,int end_col,char **board){
    int col_change = abs(end_col - start_col);
    int row_change = abs(end_row - start_row);

    // If we aren't moving as much horizontally as vertically its not diagonal
    if(col_change == row_change){
        int col_dir = (end_col > start_col) ? 1 : -1;
        int row_dir = (end_row > start_row) ? 1 : -1;

        //Chekc for obstruction
        for(int i = 1;i< row_change;i++){
            if(board[start_row + (i * row_dir)][start_col + (i * col_dir)] != EMPTY_SQUARE){
                return 0;
            }
        }
        return 1;
    }
    return 0;
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
                return 0;
            }
        }
        return 1;
    }
    // For rook moves
    else if((col_change == 0) || (row_change == 0)){
        //Check for obstruction
        for(int i=1;i<col_change + row_change;i++){
            if(board[start_row + (row_delta * row_dir * i) ][start_col + (col_delta * col_dir * i)] !=  EMPTY_SQUARE){
                return 0;
            }
        } 
        return 1;
    }
    return 0;
}
int test_king_move(int start_row,int start_col,int end_row,int end_col,char **board){
    int col_change = abs(end_col - start_col);
    int row_change = abs(end_row - start_row);

    // For regular moves
    if((col_change <= 1) && (row_change <= 1)){
        // Update the king_moves variable used to check for castling rights
        return 1;
    }

    // Check if castling is allowed
    if(row_change == 0  && col_change == 2 && start_col == 4 ){
       return test_castling(start_row,start_col,end_row,end_col,board);
    }

    return 0;
}
int test_castling(int start_row,int start_col,int end_row,int end_col,char **board){
    int col_dir = (start_col < end_col) ? 1 : -1;
    int castling_distance = (end_col == 2) ? 4 : 3;
    int rook_col = (end_col == 2) ? 0 : 1;
    int square_is_threatened = 1;

    // Check if the king has moved
    if(!(king_moves & (1U<<turn))){
        printf("King has moved, fuck you");
        return 0;
    }
    
    //Check if the rook has moved
    if(!(rook_moves & (1U<<(rook_col + turn*2)))){
        return 0;
    }
    // Check for obstuction
    for(int i = 1; i < castling_distance;i++){
        if(board[start_row][start_col + (col_dir * i)] != EMPTY_SQUARE){
            return 0;
        }
    }
    // Check that no enemy piece is threatening squares the king moves through, pretty shit but fine by now.
    for(int i = 0;i<8;i++){
        for(int j = 0;j<8;j++){
            if((turn == 0 && board[i][j] > WHITE_PIECES) || (turn == 1 && board[i][j] < WHITE_PIECES && board[i][j] != EMPTY_SQUARE)){
                square_is_threatened = test_for_threat(i,j,end_row,start_col + col_dir,board);
                if(square_is_threatened == 1){
                    return 0;
                } 
                square_is_threatened = test_for_threat(i,j,end_row,end_col,board);
                if(square_is_threatened == 1){
                    return 0;
                } 
            }
        }
    }
    // Get the rooks actual position from the earlier used rook_col variable
    rook_col = (rook_col == 0) ? 0 : 7;

    // Update the rooks position and the king_moves variable
    board[end_row][rook_col] = EMPTY_SQUARE;
    board[end_row][end_col - col_dir] = (turn == 0) ? 'R' : 'r';
    king_moves &=~ (1U<<turn);

    return 1;
}
int test_for_threat(int start_row,int start_col,int end_row,int end_col,char **board){
    int threat = 0;

    if(start_row < 0 || start_col < 0 || end_col < 0 || end_row < 0){
        return 0;
    }
    // ***This needs to be combined with the previous switch somehow***
    char char_piece = (int)board[start_row][start_col];
    char_piece = (char_piece>= 'A' && char_piece<= 'Z') ? char_piece+('a'-'A'):char_piece;
    enum pieces piece = char_piece;

    // Based on what piece we are moving we check if the move is legal
    switch (piece){
        case PAWN:
            threat = test_pawn_move(start_row,start_col,end_row,end_col,board);
            break;
        case ROOK:
            threat = test_rook_move(start_row,start_col,end_row,end_col,board);
            break;
        case KNIGHT:
            threat = test_knight_move(start_row,start_col,end_row,end_col);
            break;
        case BISHOP:
            threat = test_bishop_move(start_row,start_col,end_row,end_col,board);
            break;
        case QUEEN:
            threat = test_queen_move(start_row,start_col,end_row,end_col,board);
            break;
        case KING:
            threat = test_king_move(start_row,start_col,end_row,end_col,board);
            break;
    }
    return threat;
}
void test_for_mate(char **board){
    // Choose which king we are interested in
    Piece *king_ptr = (turn == 0) ? &white_king : &black_king;
    int check_amount = 0;
    int can_king_move = 0;
    int temp = 0;
    int temp2 = 0;
    int can_block_check = 0;
    int can_eat_threat = 0;

    // Store the king position in a temporary variable
    int king_position_row = king_ptr->row;
    int king_position_col = king_ptr->col;

    // Check if the king is threatened
    for(int i = 0;i < 8;i++){
        for(int j = 0;j < 8;j++){
            if((turn == 0 && board[i][j] > WHITE_PIECES) || (turn == 1 && board[i][j] < WHITE_PIECES && board[i][j] != EMPTY_SQUARE)){
                temp = test_for_threat(i,j,king_position_row,king_position_col,board); 
                check_amount += temp;
                if(temp == 1){
                    checking_piece.row = i;
                    checking_piece.col = j;
                    checking_piece.type = board[i][j];
                  //  printf("\n Threat %c at %d %d\n",board[i][j],i,j);
                }
                
            }
        }
    }if(check_amount != 0){
        // If in check, test if our king has any legal squares to move
        for(int i = -1;i < 2;i++){
            for(int j = -1;j < 2;j++){
                if(((king_position_row + i >= 0 ) && (king_position_row + i < 8)) && (((king_position_col + j )>= 0 ) && (king_position_col + j < 8)) &&
                (king_position_row + i != king_position_row || king_position_col + j != king_position_col)){

                    temp2 = can_king_escape(king_position_row,king_position_col,king_position_row + i,king_position_col + j,board);
                    can_king_move += temp2;
                   // printf("square %d %d ",king_position_row + i,king_position_col + j);
                    if(temp2 == 1){
                   //     printf(" XXX");
                    }
                  //  printf("\n");
                }  
            } 
        }
    }
    // In a double check if king can't move its mate
    if(check_amount == 2 && !can_king_move){
        // King can't move to safety check if we can eat checking piece or block check
        game_over = 1;
        return;
    }
    if(check_amount == 1 && !can_king_move ){
       // printf("\nFuck brah, need to block or eat\n");
        // Check if we can eat the threatening piece
        for(int i = 0;i < 8;i++){
            for(int j = 0;j < 8;j++){
                if((turn == 1 && board[i][j] > WHITE_PIECES) || (turn == 0 && board[i][j] < WHITE_PIECES && board[i][j] != EMPTY_SQUARE)){
                    can_eat_threat = test_for_threat(i,j,checking_piece.row,checking_piece.col,board);
                   // printf(" Can %c eat result: %d\n",board[i][j],can_eat_threat);
                    if(can_eat_threat){
                        return;
                    }
                }
            }
        }
        // Check if we can block whe piece

        // If its a knight we can't block so its mate
        if(checking_piece.type == 'n' || checking_piece.type == 'N'){
            game_over = 1;
            return;
        }
        int horizontal = (king_position_row == checking_piece.row) ? 1 : 0;
        int vertical = (king_position_col == checking_piece.col) ? 1 : 0;
        int diagonal = (!horizontal && !vertical) ? 1: 0;
        int col_dir = (checking_piece.col > king_position_col) ? 1 : -1;
        int row_dir = (checking_piece.row > king_position_row) ? 1 : -1;
        int row_distance = abs(king_position_row - checking_piece.row);
        int col_distance = abs(king_position_col - checking_piece.col);

       // printf("ENtering block loops %d %d %d row_dir %d col_dir %d row_dis %d col_dis %d :\n",horizontal,vertical,diagonal,row_dir,col_dir,row_distance,col_distance);
        if(diagonal){
           // printf("in diagonal\n\n");
            for(int dis = 1;dis<row_distance;dis++){
                for(int i = 0;i < 8;i++){
                    for(int j = 0;j < 8;j++){
                        if((turn == 1 && board[i][j] > WHITE_PIECES) || (turn == 0 && board[i][j] < WHITE_PIECES && board[i][j] != EMPTY_SQUARE)){
                            if(i == king_position_row && j == king_position_col){
                                break;
                            }else{
                                can_block_check += test_for_threat(i,j,king_position_row + (dis * row_dir),king_position_col + (dis * col_dir),board);
                                //printf("We are checking if %c can move to %d %d result %d\n",board[i][j],king_position_row + (dis * row_dir),king_position_col + (dis * col_dir),can_block_check);
                            }
                        }      
                }   
                }
            }
        }else if (horizontal){
            //sprintf("In horizontal\n\n");
            for(int dis = 1;dis<col_distance;dis++){
                for(int i = 0;i < 8;i++){
                    for(int j = 0;j < 8;j++){
                        if((turn == 1 && board[i][j] > WHITE_PIECES) || (turn == 0 && board[i][j] < WHITE_PIECES && board[i][j] != EMPTY_SQUARE)){
                            if(i == king_position_row && j == king_position_col){
                                break;
                            }else{
                                can_block_check += test_for_threat(i,j,king_position_row ,king_position_col + (dis * col_dir),board);
                                printf("We are checking if %c can move to %d %d result %d\n",board[i][j],king_position_row ,king_position_col + (dis * col_dir),can_block_check);
                            }
                        }
                    }
                }
            }
        } else if (vertical){
            printf("In vertical\n\n");
            for(int dis = 1;dis<row_distance;dis++){
                for(int i = 0;i < 8;i++){
                    for(int j = 0;j < 8;j++){
                        if((turn == 1 && board[i][j] > WHITE_PIECES) || (turn == 0 && board[i][j] < WHITE_PIECES && board[i][j] != EMPTY_SQUARE)){
                            if(i == king_position_row && j == king_position_col){
                                break;
                            }else{
                                can_block_check += test_for_threat(i,j,king_position_row + (dis * col_dir) ,king_position_col,board);
                                printf("Checking if %c can move to %d %d result %d\n",board[i][j],king_position_row + (dis * row_dir),king_position_col,can_block_check);
                                if(can_block_check){
                                    return;
                                }
                            }
                        }
                    }
                }
            }
        }
        if(!can_eat_threat && !can_block_check){
            game_over = 1;
            return;
        }
    }  

    

    #ifdef DEBUG
        printf("\ncan move var : %d\n check :%d",can_king_move,check_amount);
    #endif
    
}
int can_king_escape(int start_row,int start_col,int end_row,int end_col,char **board){
    int is_legal = 0;
    if(turn == 0 && (((board[end_row][end_col] < WHITE_PIECES) && (board[end_row][end_col] != EMPTY_SQUARE)) || (board[start_row][start_col] > WHITE_PIECES))){
        return 0;
    }else if(turn == 1 && ((board[end_row][end_col] > WHITE_PIECES) || (board[start_row][start_col] < WHITE_PIECES ))){
       return 0;
    }

    is_legal = test_king_move(start_row,start_col,end_row,end_col,board);
        if(is_legal == 1){
            is_legal = !test_for_checks(start_row,start_col,end_row,end_col,board);
        }
    return is_legal;
}