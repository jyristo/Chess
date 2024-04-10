#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "main.h"

int game_over = 0;
int turn = 1;
int king_moves = 3;
int move_number = 1;
int rook_moves = 15;
Move last_move = {0,0,0,0, 'X'};

Piece checking_piece = {.row = 0,.col = 0,' '};
int check_mate = 0;


int main(void){
    int move_history[100];
    int previous_move = 0;
    int move_number = 0;
    // Create the board and move variables
    char **board = NULL;
    board = (char**)malloc(8 * sizeof(char*));
    if(board == NULL){
        printf("Memory allocating failed, exitting...");
        return 1;
    }

        for(int i=0;i<8;i++){
            board[i] = (char*) malloc(8*sizeof(char));
            if(board[i] == NULL){
                printf("Memory allocating failed\n");
                for(int j=0;j<i;j++){
                    free(board[j]);
                }
            }
        }

    init_board(board);
    display(board);

    /*
    printf("\nTESTING PIECE %c\n",board[4][3]);
    printf("\n%d\n",vertical_ray(4,3,6,3,board));
    printf("\n%d\n",vertical_ray(4,3,2,3,board));
    printf("\n%d\n",horizontal_ray(4,3,4,1,board));
    printf("\n%d\n",horizontal_ray(4,3,4,5,board));
*/
    // Superloop: Ask moves and display the board until game over
    while(!game_over){
        turn ^= 1;
        ask_move(board,move_history,&previous_move,&move_number);
        display(board);
    }
    display_winner(board);
    // Destroy the board
    for(int i=0;i<8;i++){
        free(board[i]);
    }
    board = NULL;
    return 0;
}

