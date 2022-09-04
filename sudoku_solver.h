#define BOARD_SIZE 9
#define BOX_SIZE 3

typedef struct board{
    int board[BOARD_SIZE][BOARD_SIZE];
} board_t;

void display_board(board_t* board);
board_t* initalize_board(void);
int solve(board_t* board, int row, int col);
int find_missing_numbers(int row, int col, board_t* board, int* BUF, int size);
