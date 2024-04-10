#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "main.h"

void ask_move(char **board,int* move_history,int* previous_move,int* move_number){
    char start_pos[3];
    char end_pos[3];
    int correct_symbols = 0;
    int is_legal = 0;
    int move_list[100];
    int move_count = 0;
    int row,col,end_row,end_col;
    int hash = 0;

    #ifdef DEBUG
        printf("\n\nWhite %d %d\nBlack %d %d\nrook moves: %d\nking moves: %d\n",white_king.row,white_king.col,black_king.row,black_king.col,rook_moves,king_moves);
        printf("Last move was %d %d to %d %d\n",last_move.start_col,last_move.start_row,last_move.end_col,last_move.end_row);
    #endif
    if(turn == WHITE)
        printf("\n%d. White's turn\n",*move_number);
    else
        printf("\n%d. Black's turn\n",*move_number);

    compute_moves(board,move_list,&move_count,previous_move);

    if(!move_count){
        game_over = 1;
        return;
    }
    printf("Move count : %d\n",move_count);
    printf("\nKING MOVES: ");
    for(int i=0;i < move_count;i++){
        if(move_list[i] & KING_MOVE)
            printf("[%d %d],",(move_list[i] & 63), (move_list[i]>>6) & 63);
    }
    printf("\nCASTLING MOVES: ");
    for(int i=0;i < move_count;i++){
        if(move_list[i] & CASTLING_MOVE)
            printf("[%d %d]",(move_list[i] & 63), (move_list[i]>>6) & 63);
    }
    printf("\nCAPTURE MOVES: ");
    for(int i=0;i < move_count;i++){
        if(move_list[i] & CAPTURE_MOVE)
            printf("[%d %d],",(move_list[i] & 63), (move_list[i]>>6) & 63);
    }
    printf("\nROOK MOVES: ");
    for(int i=0;i < move_count;i++){
        if(move_list[i] & ROOK_MOVE)
            printf("[%d %d]",(move_list[i] & 63), (move_list[i]>>6) & 63);
    }
    printf("\nNORMAL MOVES: ");
    for(int i=0;i < move_count;i++){
        if(move_list[i] & NORMAL_MOVE)
            printf("[%d %d]",(move_list[i] & 63), (move_list[i]>>6) & 63);
    }
    printf("\nEN PASSANT MOVES: ");
    for(int i=0;i < move_count;i++){
        if(move_list[i] & EN_PASSANT_MOVE)
            printf("[%d %d]",(move_list[i] & 63), (move_list[i]>>6) & 63);
    }
    printf("\nNO HASH\n");
    for(int i=0;i < move_count;i++){
        printf("[%d]",move_list[i]);
    }
    printf("\n");
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

        col = (int)(start_pos[0]-CHAR_TO_INT); 
        row = atoi(&(start_pos[1]))-1;
        end_col = (int)(end_pos[0]-CHAR_TO_INT);
        end_row = atoi(&(end_pos[1]))-1;
        // printf("Our move: %d %dto %d %d\n",row,col,end_row,end_col);

        hash = test_move(board,row,col,end_row,end_col,&move_count,move_list);
        if(hash){
            make_move(board,hash);
            is_legal = 1;
            //printf("YIPPII\n");
        }else{
            printf("ILLEGAL MOVE!!!\n");
        }
    }
    *previous_move = hash;
    move_history[(*move_number)++] = hash;
    printf("Previous moves: ");
    for(int i = 0; i < *move_number;i++){
        printf("[%d]",move_history[i]);
    }
}
int test_move(char **board,int row,int col,int end_row,int end_col,int* move_count,int* move_list){
    // Convert string coordinates to integers that match the board array
    // CHAR_TO_INT is used to convert the column value (a-h) to a char number, then atoi for integer conversion
    printf("IN TEST MOVE\n");
    int hash = ((row * 8 + col)  | ((end_row * 8 + end_col)<<6));
    printf("HASH: %d move count: %d\n",hash,*move_count);
    for(int i = 0;i < *move_count;i++){
        if((move_list[i] & 0xFFF) == hash)
            return move_list[i];
    }
    return 0;
}

void make_move(char** board,int hash){
    int row = (hash & 0x3F) / 8;
    int col = (hash & (0x3F)  % 8);
    int end_row = (hash>>6 & 0x3F) / 8;
    int end_col = (hash>>6 & (0x3F) % 8);

    board[end_row][end_col] = board[row][col];
    board[row][col] = EMPTY_SQUARE;

    if(hash & CASTLING_MOVE){
        make_castling_move(board,row,col,end_row,end_col);
    }else if(hash & ROOK_MOVE){
        int rook_bit = (col == 0) ? 0 : 2;
        rook_moves &=~ (1U<<(turn + rook_bit));
        printf("Rook moves is now %d\n",rook_moves);
    }else if(hash & KING_MOVE){
        king_moves |= (1U<<turn);
    }else if(hash & EN_PASSANT_MOVE){
        board[row][end_col] = EMPTY_SQUARE;
    }

}
void make_castling_move(char** board,int row,int col,int end_row,int end_col){
    if(end_col == 2){
        board[row][3] = board[row][0];
        board[row][0] = EMPTY_SQUARE;
        
    }else{
        board[row][5] = board[row][7];
        board[row][7] = EMPTY_SQUARE;
    }
    king_moves &=~ (1U<<turn);
}
void add_move(int row,int col,int end_row,int end_col,int* moves,int* move_count,int info){
    int move = (row * 8 + col) | ((end_row * 8 + end_col)<<6) | info;
    //printf("Move_count: %d adding move %d %d to %d %d\n",*move_count,row,col,end_row,end_col);
    moves[*move_count] = move;
    (*move_count)++;
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
    //If we are not in same column early exit
    if(col != new_col)
        return 0;
    // Check the status of all squares in between, if we hit something return 0
    while(row != new_row && row < 8 && row >= 0){
        if(board[row][col] != EMPTY_SQUARE)
            return 0;
        row += dir;
    }
    // Return 1 if we are in the target square
    if(row == new_row)
        return 1;
    return 0;
}
int horizontal_ray(int row,int col,int new_row,int new_col,char** board){
    int dir = (new_col > col) ? 1 : -1;
    col += dir;
    // If not we are not in the same row early exit
    if(row != new_row)
        return 0;
    // Check the status of all squares in between, if we hit something return 1
    while(col != new_col && col < 8 && col >= 0){
        if(board[row][col] != EMPTY_SQUARE)
            return 0;
        col += dir;
    }
    // Return 1 if we are in the target square
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
    // Check one square diagonally for both directions 
    if(new_row == row + dir && (new_col == col + 1 || new_col == col - 1))
        return 1;
    return 0;
}
int test_knight_threat(int row,int col,int king_row,int king_col,char** board){
    int row_distance = abs(king_row - row);
    int col_distance = abs(king_col - col);
    // if distance is 1 & 2 or 2 & 1 we have a hit
    if((row_distance == 1 && col_distance == 2) || (row_distance == 2 && col_distance == 1))
        return 1;
    return 0;
}
int test_king_threat(int row,int col,int king_row,int king_col,char** board){
    int row_distance = abs(king_row - row);
    int col_distance = abs(king_col - col);
    // If distance is 1 in any direction we have a hit
    if(row_distance <= 1 && col_distance <= 1)
        return 1;
    return 0;
}
int diagonal_ray(int row,int col,int new_row,int new_col,char** board){
    int row_dir = (new_row > row) ? 1 : -1;
    int col_dir = (new_col > col) ? 1 : -1;
    col += col_dir;
    row += row_dir;
    // If we land in the same row or column we stop looping or if we hit a piece we can exit
    while(col != new_col && row != new_row && col < 8 && col >= 0 && row < 8 && row  >= 0 ){
        if(board[row][col] != EMPTY_SQUARE){
            return 0;
        }
        col += col_dir;
        row += row_dir;
    }
    // If we get into target square we have a hit
    if(row == new_row && col == new_col)
        return 1;
    return 0;
}
int is_king_threatened(int row,int col,int new_row,int new_col,char** board,int testing_for_king){
    int king_row;
    int king_col;
    // If we are moving the king, we have to refererence the new squares
    if(testing_for_king){
       king_row = new_row;
       king_col = new_col;
    }else{
        king_row = (turn == WHITE) ? white_king.row : black_king.row;
        king_col = (turn == WHITE) ? white_king.col : black_king.col;
    }
    int threat = 0;
    // Pretend to make the move on the board
    char previous_piece = board[new_row][new_col];
    board[new_row][new_col] = board[row][col];
    board[row][col] = EMPTY_SQUARE;
    // Run through the pieces on the board, if enemy piece we check if it can attack the traget square(King)
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
                    threat = test_king_threat(i,j,king_row,king_col,board);
                }
            }
            if(threat){
                board[row][col] = board[new_row][new_col];
                board[new_row][new_col] = previous_piece;
                return threat;
            }
        }
    }
    // Place the board to its original position
    board[row][col] = board[new_row][new_col];
    board[new_row][new_col] = previous_piece;
    //printf("NO THREAT\n");

    return threat;
}
void compute_moves(char** board,int* moves,int* move_count,int* previous_move){
    for(int i = 0;i < 8;i++){
        for(int j = 0;j < 8;j++){
            int current_square = get_square_status(board,i,j);
            if(current_square == FRIENDLY_PIECE){
                int current_piece = board[i][j];
                current_piece = (current_piece >= 'A' && current_piece <= 'Z') ? current_piece + ('a'-'A'):current_piece;
                enum pieces piece = current_piece;
                // printf("Computing piece %c for turn %d\n",current_piece,turn);
                switch (piece){
                case PAWN:
                   compute_pawn_moves(i,j,board,moves,move_count,previous_move);
                    break;
                case ROOK:
                    compute_rook_moves(i,j,board,moves,move_count);
                    break;
                case KNIGHT:
                   compute_knight_moves(i,j,board,moves,move_count);
                    break;
                case BISHOP:
                    compute_bishop_moves(i,j,board,moves,move_count);
                    break;
                case QUEEN:
                    compute_queen_moves(i,j,board,moves,move_count);
                    break;
                case KING:
                    compute_king_moves(i,j,board,moves,move_count);
                    break;
                }
            }
        }
    }
}
void compute_pawn_moves(int row,int col,char** board,int* moves, int* move_count,int* previous_move){
   // printf("IN PAWN\n");
    compute_forward_moves(row,col,board,moves,move_count);
    //printf("In capture\n");
    compute_capture_moves(row,col,board,moves,move_count);
    compute_en_passant_moves(row,col,board,moves,move_count,previous_move);
}
void compute_forward_moves(int row,int col,char** board,int* moves,int* move_count){
    int first_move = ((turn == WHITE && row == 1) || (turn == BLACK && row == 6)) ? 1 : 0;
    int move_dir = (turn == WHITE) ? 1 : -1;
    // Single row moves
    if(board[row + move_dir][col] == EMPTY_SQUARE  && !is_king_threatened(row,col,row + move_dir,col,board,0)){
        add_move(row,col,row + move_dir,col,moves,move_count,NORMAL_MOVE);
     // Two row moves
        if(first_move && board[row + move_dir*2][col] == EMPTY_SQUARE && !is_king_threatened(row,col,row + move_dir*2,col,board,0))
            add_move(row,col,row + move_dir*2,col,moves,move_count,PAWN_DOUBLE_MOVE);
    }
}
void compute_capture_moves(int row,int col,char** board,int* moves,int* move_count){
    int move_dir = (turn == WHITE) ? 1 : -1;
    int left_col = col - 1;
    int right_col = col + 1;
    // Test left diagonal
    if(left_col < 8 && left_col >=0){
        int left_piece = get_square_status(board,row + move_dir,left_col);
        if(left_piece == ENEMY_PIECE && !is_king_threatened(row,col,row + move_dir,left_col,board,0)){
            printf("Added move %d %d %d %d\n",row,col,row + move_dir,left_col);
            add_move(row,col,row + move_dir,left_col,moves,move_count,CAPTURE_MOVE);
        }
    // Test right diagonal
    }if(right_col < 8 && right_col >= 0){
        int right_piece = get_square_status(board,row + move_dir,right_col);
        if((right_piece == ENEMY_PIECE && !is_king_threatened(row,col,row + move_dir,right_col,board,0))){
            printf("Added move %d %d %d %d\n",row,col,row + move_dir,right_col);
            add_move(row,col,row + move_dir,right_col,moves,move_count,CAPTURE_MOVE);
        }
    }
}
void compute_en_passant_moves(int row,int col,char** board,int* moves,int* move_count,int* previous_move){
    int prev_move_row = ((*previous_move)>>6 & 0x3F) / 8;
    int prev_move_col = ((*previous_move)>>6 & 0x3F) % 8;
    // If previous move was a pawn double move and the column is adjacent to ours its legal
    if((*previous_move & PAWN_DOUBLE_MOVE) && abs(prev_move_col - col) == 1 && prev_move_row == row ){
        int dir = (turn == WHITE) ? 1 : -1;
        printf("ADDING PASSAANT MOVE\n");
        add_move(row,col,row + dir,prev_move_col,moves,move_count,EN_PASSANT_MOVE);
    }
}
void compute_knight_moves(int row,int col,char** board,int* moves,int* move_count){
    for(int dir1 = -1; dir1 <= 1;dir1 +=2){
        for(int dir2 = -1; dir2 <= 1;dir2 +=2){
            int new_row = row + 2 * dir1;
            int new_col = col + 1 * dir2;
            if(new_row >= 0 && new_row < 8 && new_col >= 0 && new_col < 8 && !is_king_threatened(row,col,new_row,new_row,board,0)){
                int current_square = get_square_status(board,new_row,new_col);
                int move_info = (current_square == ENEMY_PIECE) ? CAPTURE_MOVE : NORMAL_MOVE;
                if(current_square != FRIENDLY_PIECE)
                    add_move(row,col,new_row,new_col,moves,move_count,move_info);
            }
            new_row =  row + (1*dir1);
            new_col = col + (2*dir2);
            if(new_row >= 0 && new_row < 8 && new_col >= 0 && new_col < 8 && !is_king_threatened(row,col,new_row,new_row,board,0)){
                int free_square = get_square_status(board,new_row,new_col);
                int move_info = (free_square == ENEMY_PIECE) ? CAPTURE_MOVE : NORMAL_MOVE;
                if(free_square != FRIENDLY_PIECE)
                    add_move(row,col,new_row,new_col,moves,move_count,move_info);
            }
        }
    }
}
void compute_rook_moves(int row,int col,char** board,int* moves,int* move_count){
    //printf("Entered horixontal\n");
    compute_horizontal_moves(row,col,board,moves,move_count,ROOK_MOVE);
   // printf("ENTERED veritcal\n");
    compute_vertical_moves(row,col,board,moves,move_count,ROOK_MOVE);
}
void compute_horizontal_moves(int row,int col,char** board,int* moves,int* move_count,int info){
    for(int dir = -1; dir <= 1;dir += 2){
        int new_col = col + dir;
        int current_square = EMPTY_SQUARE;
        while(new_col < 8 && new_col >= 0 && current_square == EMPTY_SQUARE && !is_king_threatened(row,col,row,new_col,board,0)){
            current_square = get_square_status(board,row,new_col);
            int move_info = (current_square == ENEMY_PIECE) ? CAPTURE_MOVE | info : NORMAL_MOVE | info;
            if(current_square == EMPTY_SQUARE || current_square == ENEMY_PIECE){
                //printf("adding move %d %d to %d %d\n",row,col,row,new_col);
                add_move(row,col,row,new_col,moves,move_count,move_info);
            }
            //printf("Added move\n");
            new_col += dir;
        }
    }
}
void compute_vertical_moves(int row,int col,char** board,int* moves,int* move_count, int info){
    for(int dir = -1; dir <= 1;dir += 2){
        int current_square = EMPTY_SQUARE;
        int new_row = row + dir;
        while(new_row < 8 && new_row >= 0 && current_square == EMPTY_SQUARE && !is_king_threatened(row,col,new_row,col,board,0)){
            current_square = get_square_status(board,new_row,col);
            int move_info = (current_square == ENEMY_PIECE) ? CAPTURE_MOVE | info : NORMAL_MOVE | info;
            if(current_square == EMPTY_SQUARE || current_square == ENEMY_PIECE)
                add_move(row,col,new_row,col,moves,move_count,move_info);
            new_row += dir;
            //printf("IN THIS BITCH %d %d\n",new_row,col);
        }
    }
}
void compute_diagonal_moves(int row,int col,char** board,int* moves,int* move_count){
    for(int dir1 = -1;dir1 <= 1; dir1 += 2){
        for(int dir2 = -1;dir2 <= 1;dir2 += 2){
            int current_square = EMPTY_SQUARE;
            int new_row = row + dir1;
            int new_col = col + dir2;
            while(new_row < 8 && new_row >= 0 && new_col < 8 && new_col >= 0 && current_square == EMPTY_SQUARE &&
                !is_king_threatened(row,col,new_row,new_col,board,0)){

                current_square = get_square_status(board,new_row,new_col);
                int move_info = (current_square == ENEMY_PIECE) ? CAPTURE_MOVE : NORMAL_MOVE;
                if(current_square == EMPTY_SQUARE || current_square == ENEMY_PIECE){
                    add_move(row,col,new_row,new_col,moves,move_count,move_info);
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
                int move_info = (current_square == ENEMY_PIECE) ? CAPTURE_MOVE | KING_MOVE: KING_MOVE;
                if(current_square != FRIENDLY_PIECE && !is_king_threatened(row,col,new_row,new_col,board,1)){
                add_move(row,col,new_row,new_col,moves,move_count,move_info);
                }
            }
        }
    }
    if(king_moves & (1U<<turn))
        compute_castling(row,col,board,moves,move_count);
}
void compute_castling(int row,int col,char** board,int* moves,int* move_count){
    int current_square = EMPTY_SQUARE;
    int castling_rights = 1;
    int move_info = KING_MOVE | CASTLING_MOVE;
    //printf("IN CASTLING\n");
    if(rook_moves & 1U<<turn){
        for(int new_col = col - 1; new_col >= 1;new_col--){
            current_square = get_square_status(board,row,new_col);

            if(current_square != EMPTY_SQUARE || is_king_threatened(row,col,row,new_col,board,1))
                castling_rights = 0;
        }
        if(castling_rights)
            add_move(row,col,row,col - 2,moves,move_count,move_info);
    }
    castling_rights = 1;
    // Test for other direction
    if(rook_moves & 1U<<(turn + 2)){
    for(int new_col = col + 1; new_col <= 6;new_col++){
            current_square = get_square_status(board,row,new_col);
            if(current_square != EMPTY_SQUARE || is_king_threatened(row,col,row,new_col,board,1))
                castling_rights = 0;
        }
        if(castling_rights)
            add_move(row,col,row,col + 2,moves,move_count,move_info);
        
    }
}
void compute_bishop_moves(int row,int col,char** board,int* moves,int* move_count){
    compute_diagonal_moves(row,col,board,moves,move_count);
}
void compute_queen_moves(int row,int col,char** board,int* moves,int* move_count){
    //printf("Entered queen rook\n");
    compute_vertical_moves(row,col,board,moves,move_count,NORMAL_MOVE);
    compute_horizontal_moves(row,col,board,moves,move_count,NORMAL_MOVE);
    //printf("Entered queen bishop\n");
    compute_diagonal_moves(row,col,board,moves,move_count);
}
