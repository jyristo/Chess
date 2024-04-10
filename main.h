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

#define NORMAL_MOVE         (1U<<12)
#define KING_MOVE           (1U<<13)
#define CASTLING_MOVE       (1U<<14)
#define EN_PASSANT_MOVE     (1U<<15)
#define PAWN_DOUBLE_MOVE    (1U<<16)
#define ROOK_MOVE           (1U<<17)
#define CAPTURE_MOVE        (1U<<18)



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
extern int move_number;
extern int rook_moves;
extern int check_mate;

void init_board(char **board);
void display(char **board);
void display_winner(char** board);
void ask_move(char **board,int* move_history,int* previous_move,int* move_number);


int get_square_status(char** board,int row, int col);
void compute_moves(char** board,int* moves,int* move_count,int* previous_move);
void compute_pawn_moves(int row,int col,char **board,int* moves, int* move_count,int* previous_move);
void compute_forward_moves(int row,int col,char** board,int* moves,int* move_count);
void compute_capture_moves(int row,int col,char** board,int* moves,int* move_count);
void compute_en_passant_moves(int row,int col,char** board,int* moves,int* move_count,int* previous_move);
void compute_knight_moves(int row,int col,char** board,int* moves,int* move_count);
void compute_rook_moves(int row,int col,char** board,int* moves,int* move_count);
void compute_bishop_moves(int row,int col,char** board,int* moves,int* move_count);
void compute_queen_moves(int row,int col,char** board,int* moves,int* move_count);
void compute_king_moves(int row,int col,char** board,int* moves,int* move_count);
void compute_diagonal_moves(int row,int col,char** board,int* moves,int* move_count);
void compute_horizontal_moves(int row,int col,char** board,int* moves,int* move_count,int info);
void compute_vertical_moves(int row,int col,char** board,int* moves,int* move_count,int ino);
void compute_castling(int row,int col,char** board,int* moves,int* move_count);
int is_king_threatened(int row,int col,int new_row,int new_col,char** board,int testing_for_king);

void add_move(int row,int col,int end_row,int end_col,int* moves,int* move_count,int move_infos);
void make_move(char** board,int hash);
void make_castling_move(char** board,int row,int col,int end_row,int end_col);
int test_move(char **board,int row,int col,int end_row,int end_col,int* move_count,int* move_list);

int vertical_ray(int row,int col,int new_row,int new_col,char** board);
int horizontal_ray(int row,int col,int new_row,int new_col,char** board);
int diagonal_ray(int row,int col,int new_row,int new_col,char** board);





#endif