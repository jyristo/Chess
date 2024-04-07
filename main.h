#ifndef MAIN_H
#define MAIN_H

#define CHAR_TO_INT 97
#define WHITE_PIECES 96
#define EMPTY_SQUARE ' '
#define ENEMY_PIECE 999
#define FRIENDLY_PIECE 888
#define WHITE 0
#define BLACK 1
#define ERROR -1

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
    char type;
}Piece;

enum pieces{
    PAWN = 'p',
    ROOK = 'r',
    KNIGHT = 'n',
    BISHOP = 'b',
    QUEEN = 'q',
    KING ='k'
};


extern Move last_move;
extern Piece white_king;
extern Piece black_king;
extern Piece temp_king;
extern Piece checking_piece;
extern int game_over;
extern int king_moves;
extern int turn;
extern int move_count;
extern int rook_moves;
extern int check_mate;
extern int move_count;

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

int can_king_escape(int start_row,int start_col,int end_row,int end_col,char **board);
int test_for_threat(int start_row,int start_col,int end_row,int end_col,char **board);
int test_for_checks(int start_row,int start_col,int end_row,int end_col,char **board);
void finalize_move(int start_row,int start_col,int end_row,int end_col,char **board);
void test_for_mate(char **board);

int get_square_status(char** board,int row, int col);
void compute_moves(char** board,int* moves,int* move_count);
void compute_pawn_moves(int row,int col,char **board,int* moves, int* move_count);
void compute_forward_moves(int row,int col,char** board,int* moves,int* move_count);
void compute_capture_moves(int row,int col,char** board,int* moves,int* move_count);
void compute_en_passant_moves(int row,int col,char** board,int* moves,int* move_count);
void compute_knight_moves(int row,int col,char** board,int* moves,int* move_count);
void compute_rook_moves(int row,int col,char** board,int* moves,int* move_count);
void compute_bishop_moves(int row,int col,char** board,int* moves,int* move_count);
void compute_queen_moves(int row,int col,char** board,int* moves,int* move_count);
void compute_king_moves(int row,int col,char** board,int* moves,int* move_count);
void compute_diagonal_moves(int row,int col,char** board,int* moves,int* move_count);
void compute_horizontal_moves(int row,int col,char** board,int* moves,int* move_count);
void compute_vertical_moves(int row,int col,char** board,int* moves,int* move_count);
void compute_castling(int row,int col,char** board,int* moves,int* move_count);
int test_king_safety(int row,int col,int new_row,int new_col,char** board);
void add_move(int start_row,int start_col,int end_row,int end_col,int* moves,int* move_count);

int vertical_ray(int row,int col,int new_row,int new_col,char** board);
int horizontal_ray(int row,int col,int new_row,int new_col,char** board);
int diagonal_ray(int row,int col,int new_row,int new_col,char** board);





#endif