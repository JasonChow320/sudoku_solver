#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sudoku_solver.h"

int main(void){

    board_t* board = initalize_board();
    if(board == NULL){
        return 1;
    }

    printf("solving sudoku puzzle...\n");
    solve(board, 0, 0);

    printf("Finished\n");
    display_board(board);
    
    free(board);
    return 0;
}

board_t* initalize_board(){
    
    FILE* fp = fopen("input.txt", "r");
    if(fp == NULL){
        printf("Unable to open file, exiting\n");
        return NULL;
    }
    
    int BUF_SIZE = 64, line_counter = 0;
    char BUF[BUF_SIZE];

    board_t* board = (board_t*) malloc(sizeof(board_t));
    
    for (int i = 0; i < BOARD_SIZE; i++){
        for (int j = 0; j < BOARD_SIZE; j++){
            board->board[i][j] = 0;
        }
    }

    while (fgets(BUF, BUF_SIZE, fp) != NULL){

        if (++line_counter > 9){

            printf("Invalid input size, has to be 9 lines\n");
            free(board);
            free(fp);
            return NULL;
        }
        if(strlen(BUF) != 10){

            printf("Invalid size for line %d\n", line_counter);
            free(fp);
            free(board);
            return NULL;
        }

        for (int i = 0; i < BOARD_SIZE; i++){
            if(BUF[i] < '0' || BUF[i] > '9'){

                printf("Invalid input, has to be a number\n");
                free(board);
                free(fp);
                return NULL;
            }
            board->board[line_counter-1][i] = (int)BUF[i] - '0';
        } 
    }
    free(fp);
    
    if(line_counter != 9){

        printf("Invalid size, has to be 9 lines :(\n");
        free(board);
        return NULL;
    }
 
    return board;
}

void display_board(board_t* board){
    
    char* bar = "\n----------------------\n";
    printf("%s", bar);

    for (int i = 0; i < BOARD_SIZE; i++){
        for (int j = 0; j < BOARD_SIZE; j++){

            if(j != 0 && j % BOX_SIZE == 0){
                printf("| ");
            }
            printf("%d ", board->board[i][j]);
        }

        if(i != 0 && (i+1) % BOX_SIZE == 0){
            printf("%s", bar);
        } else {
            printf("\n");
        }
    }
}

int solve(board_t* board, int row, int col){

    if (row == 9){
        return 0;
    }

    int BUF[BOARD_SIZE] = { 0 };
    if(board->board[row][col] != 0){

        if (col >= 8){
            return solve(board, row + 1, 0);
        } else {
            return solve(board, row, col+1);
        }
    } else {

        board->board[row][col] = 0;
        if(find_missing_numbers(row, col, board, BUF, BOARD_SIZE) == 0){
            return 1;
        }

        for(int i = 0; i < BOARD_SIZE; i++){
            if(BUF[i] == 0){

                // try inserting the missing number and see if it works
                board->board[row][col] = i+1;
                if(col >= 8 && solve(board, row + 1, 0) == 0){
                    return 0;
                } else if(solve(board, row, col+1) == 0){
                    return 0;
                }
            }
        }    

        board->board[row][col] = 0; 
        return 1;
    }
}

int find_missing_numbers(int row, int col, board_t* board, int* BUF, int size){
    
    int count = 9;
    
    for (int i = 0; i < size; i++){
        BUF[i] = 0;
    }

    for (int row_box = 0; row_box < BOX_SIZE; row_box++){
        for (int col_box = 0; col_box < BOX_SIZE; col_box++){
            
            int row_start = row_box*BOX_SIZE, row_end = (row_box+1) * BOX_SIZE,
                col_start = col_box*BOX_SIZE, col_end = (col_box+1) * BOX_SIZE;

            // look for the same square
            if (row >= row_start && row < row_end
                && col >= col_start && col < ((col_box+1) * BOX_SIZE)){
                        
                // check everyin in same square
                for (int i = row_start; i < row_end; i++){
                    for (int j = col_start; j < col_end; j++){
                        if (board->board[i][j] != 0){
                            if (BUF[board->board[i][j] - 1] == 0)
                                count--;
                            BUF[board->board[i][j] - 1] = 1;
                        }
                    }
                }
                
                // now check row + col
                for (int i = 0; i < BOARD_SIZE; i++){
                    for (int j = 0; j < BOARD_SIZE; j++){
                        if (i >= row_start && i < row_end && j >= col_start && j < col_end){
                            continue;
                        }
                        
                        if(i == row || j == col){
                            if (board->board[i][j] != 0){
                                if (BUF[board->board[i][j] - 1] == 0){
                                    count--;
                                }
                                BUF[board->board[i][j] - 1] = 1;
                            } 
                        }
                    }
                }
            }
        }
    }
    
    return count;
}

