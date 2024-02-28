#ifndef MAIN_H
#define MAIN_H

#define CHAR_TO_INT 97
#define WHITE_PIECES 96
#define EMPTY_SQUARE ' '

typedef struct{
    int start_col;
    int start_row;
    int end_col;
    int end_row;
    char piece; 
}Move;


typedef struct{
    int row;
    int col;
}Piece;

enum pieces{
    PAWN = 112,
    ROOK = 114,
    KNIGHT = 110,
    BISHOP = 98,
    QUEEN = 113,
    KING = 107
};


extern Move last_move;
extern Piece white_king;
extern Piece black_king;
extern Piece temp_king;
extern int game_over;
extern int king_moves;
extern int turn;
extern int move_count;
extern int rook_moves;
extern int check_mate;

void init_board(char **board);
void display(char **board);
void display_winner();
int check_move_legality(char **board,char start[],char end[],int turn);
void ask_move(char **board);

int test_pawn_move(int start_row,int start_col,int end_row,int end_pos,char **board);
int test_rook_move(int start_row,int start_col,int end_row,int end_col,char **board);
int test_knight_move(int start_row,int start_col,int end_row,int end_col);
int test_bishop_move(int start_row,int start_col,int end_row,int end_col,char **board);
int test_queen_move(int start_row,int start_col,int end_row,int end_col,char **board);
int test_king_move(int start_row,int start_col,int end_row,int end_col,char **board);
int test_castling(int start_row,int start_col,int end_row,int end_col,char **board);

int test_for_threat(int start_row,int start_col,int end_row,int end_col,char **board);
int test_for_checks(int start_row,int start_col,int end_row,int end_col,char **board);
void finalize_move(int start_row,int start_col,int end_row,int end_col,char **board);
void test_for_mate(char **board);



#endif