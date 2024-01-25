#ifndef MAIN_H
#define MAIN_H

#define CHAR_TO_INT 97
#define WHITE_PIECES 96
#define EMPTY_SQUARE ' '
void init_board(char** board);
void display(char **board);

int check_move_legality(char **board,char start[],char end[],int turn);
void ask_move(char **board,int turn);

int test_pawn_move(int start_row,int start_col,int end_row,int end_pos,int turn,char **board);
int test_rook_move(int start_row,int start_col,int end_row,int end_col,char **board);
int test_knight_move(int start_row,int start_col,int end_row,int end_col);

enum pieces{
    PAWN = 112,
    ROOK = 114,
    KNIGHT = 110,
    BISHOP = 98,
    QUEEN = 113,
    KING = 107
};
int gameover;
int turn;
char move[5];

#endif