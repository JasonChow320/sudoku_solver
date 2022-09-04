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
    
    int BUF_SIZE = 64, counter = 0;
    char BUF[BUF_SIZE];

    board_t* board = (board_t*) malloc(sizeof(board_t));
    
    for (int i = 0; i < BOARD_SIZE; i++){
        for (int y = 0; y < BOARD_SIZE; y++){
            board->board[i][y] = 0;
        }
    }

    while (fgets(BUF, BUF_SIZE, fp) != NULL){
        
        if (++counter > 9){
            printf("Invalid input size, has to be 9 lines\n");
            free(board);
            free(fp);
            return NULL;
        }
        if(strlen(BUF) != 10){
            printf("INvalid size");
            free(fp);
            free(board);
            return NULL;
        }
        for (int i = 0; i < BOARD_SIZE; i++){
            if(BUF[i] < 48 || BUF[i] > 57){
                printf("Invalid input, has to be a number\n");
                free(board);
                free(fp);
                return NULL;
            }
            board->board[counter-1][i] = (int)BUF[i] - 48;
        } 
    }
    free(fp);
    
    if(counter != 9){
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
        for (int y = 0; y < BOARD_SIZE; y++){
            if(y != 0 && y % BOX_SIZE == 0){
                printf("| ");
            }
            printf("%d ", board->board[i][y]);
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
                for (int y = col_start; y < col_end; y++){
                    if (board->board[i][y] != 0){
                        if (BUF[board->board[i][y] - 1] == 0)
                            count--;
                        BUF[board->board[i][y] - 1] = 1;
                    }
                }
            }
            
            // now check row + col
            for (int i = 0; i < BOARD_SIZE; i++){
                for (int y = 0; y < BOARD_SIZE; y++){
                    if (i >= row_start && i < row_end && y >= col_start && y < col_end)
                        continue;
                    
                    if(i == row || y == col){
                        if (board->board[i][y] != 0){
                            if (BUF[board->board[i][y] - 1] == 0)
                                count--;
                            BUF[board->board[i][y] - 1] = 1;
                        } 
                    }
                }
            }
        }
    }}
    
    return count;
}


