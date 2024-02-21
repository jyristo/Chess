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
    uint64_t pawns;
    uint64_t rooks;
    uint64_t knights;
    uint64_t bishops;
    uint64_t queen;
    uint64_t king;
}Pieces;

enum pieces{
    PAWN = 112,
    ROOK = 114,
    KNIGHT = 110,
    BISHOP = 98,
    QUEEN = 113,
    KING = 107
};

extern Pieces black_pieces;
extern Pieces white_pieces;
extern Move last_move;
extern int white_king_pos;
extern int black_king_pos;
extern int gameover;
extern int king_moves;
extern int turn;
extern int move_count;
extern int rook_moves;
extern int black pieces[]
extern int 


void init_board(char** board);
void display(char **board);

int check_move_legality(char **board,char start[],char end[],int turn);
void ask_move(char **board,int turn);

int test_pawn_move(int start_row,int start_col,int end_row,int end_pos,int turn,char **board);
int test_rook_move(int start_row,int start_col,int end_row,int end_col,char **board);
int test_knight_move(int start_row,int start_col,int end_row,int end_col);
int test_bishop_move(int start_row,int start_col,int end_row,int end_col,char **board);
int test_queen_move(int start_row,int start_col,int end_row,int end_col,char **board);
int test_king_move(int start_row,int start_col,int end_row,int end_col,char **board);
int test_castling(int start_row,int start_col,int end_row,int end_col,char **board);

int check_for_threat(int start_row,int start_col,int end_row,int end_col,char **board);
int test_for_checks(int start_row,int start_col,int end_row,int end_col,char **board);
void finalize_move(int start_row,int start_col,int end_row,int end_col,char **board);


#endif