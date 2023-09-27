/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Lihi Cohen					*
* Date: 20.04.2023							*
* Description: Source file for knight		*
*********************************************/

/************************************LIBRARIES**************************************************/
#include <assert.h> /*assert*/
#include <time.h> /*clock(), time_t*/

#include "knight.h"
#include "heap.h"
#include "utilities.h"

/********************************FORWARD DECLERATIONS*******************************************/

#define TOTAL_STEPS (SIZE_COL * SIZE_ROW)
#define MOVES (8)
#define TIME_OUT (2)

enum moves
{
    X_AXIS = 0,
    Y_AXIS = 1,
    NUM_OF_AX = 2
};

typedef struct options
{
    int options;
    int row;
    int col;
}options_t;

static int KnightsTourNaive(int matrix[SIZE_ROW][SIZE_COL], int knight_moves[][NUM_OF_AX], int row, int col, time_t start_time);

static int KnightsTourWarnsdorff(int matrix[SIZE_ROW][SIZE_COL], int knight_moves[][NUM_OF_AX], int row, int col);

static int MoveIsValid(int sol[SIZE_ROW][SIZE_COL], int point_x, int point_y);

static size_t CountValidMoves(int sol[SIZE_ROW][SIZE_COL], int knights_move[][NUM_OF_AX], int point_x, int point_y);

static void InitKnightsMoves(int knights_moves[][NUM_OF_AX]);

static void ClearBoard(int matrix[SIZE_ROW][SIZE_COL]);

static int MinHeapCmp(const void *lhs, const void *rhs);

/************************************Functions***************************************************/

int KnightTour(int matrix[SIZE_ROW][SIZE_COL],int row, int col)
{
    time_t start = clock();
    int knights_moves[MOVES][NUM_OF_AX] = {0};

    assert(NULL != matrix);

    ClearBoard(matrix);
    InitKnightsMoves(knights_moves);

    matrix[row][col] = 1;

    return KnightsTourWarnsdorff(matrix, knights_moves, row, col);
}

void PrintBoard(int solution[SIZE_ROW][SIZE_COL])
{
    size_t i = 0, j = 0;

    for (i = 0; i < SIZE_ROW; ++i)
    {
        for (j = 0; j < SIZE_COL; j++)
        {
            printf(" %4d ", solution[i][j]);
        }
        printf("\n\n");
    }
}

/************************************ Static Functions *******************************************/

static int KnightsTourNaive(int matrix[SIZE_ROW][SIZE_COL], int knight_moves[][NUM_OF_AX], int row, int col, time_t start_time)
{
    size_t i = 0;
    size_t new_row = 0;
    size_t new_col = 0;
    time_t current_time = 0;
    time_t elapsed_time = 0;

    if (matrix[row][col] == TOTAL_STEPS)
    {
        return SUCCESS;
    }
    
    for (i = 0; i < MOVES; ++i)
    {
        new_row = row + knight_moves[i][X_AXIS];
        new_col = col + knight_moves[i][Y_AXIS];

        current_time = clock(); 
        elapsed_time = (double)(current_time - start_time) / CLOCKS_PER_SEC;
        if (elapsed_time > TIME_LIMIT) 
        {
            return TIME_OUT; 
        }

        if(TRUE == MoveIsValid(matrix, new_row, new_col))
        {
            matrix[new_row][new_col] = matrix[row][col] + 1; 

            if (SUCCESS == KnightsTourNaive(matrix, knight_moves, new_row, new_col, start_time))
            {
                return SUCCESS;
            }
            else
            {
                matrix[new_row][new_col] = 0;
            }
        }
    }

    return FAILURE;
}

static int KnightsTourWarnsdorff(int matrix[SIZE_ROW][SIZE_COL], int knight_moves[][NUM_OF_AX], int row, int col)
{
    heap_t *heap = HeapCreate(MinHeapCmp);
    size_t i = 0;
    size_t new_row = 0;
    size_t new_col = 0;
    
    options_t pos_moves[MOVES] = {0};

    if (matrix[row][col] == TOTAL_STEPS)
    {
        HeapDestroy(heap);
        return SUCCESS;
    }

    for (i = 0; i < MOVES; ++i)
    {
        pos_moves[i].row = row + knight_moves[i][X_AXIS];
        pos_moves[i].col = col + knight_moves[i][Y_AXIS];

        if(MoveIsValid(matrix, pos_moves[i].row, pos_moves[i].col))
        {
            pos_moves[i].options = CountValidMoves(matrix, knight_moves, pos_moves[i].row, pos_moves[i].col);
            HeapPush(heap, &pos_moves[i]);
        }
    }

    while (!HeapIsEmpty(heap))
    {
        new_row = ((options_t *)HeapPeek(heap))->row;
        new_col = ((options_t *)HeapPeek(heap))->col;
        HeapPop(heap);

        matrix[new_row][new_col] = matrix[row][col] + 1;

        if(SUCCESS == KnightsTourWarnsdorff(matrix, knight_moves, new_row, new_col))
        {
            HeapDestroy(heap);
            return SUCCESS;
        }
        else
        {
            matrix[new_row][new_col] = 0;
        }
    }

    HeapDestroy(heap);
    return FAILURE;
}

static int MoveIsValid(int sol[SIZE_ROW][SIZE_COL],int point_x, int point_y)
{
    return ((point_x >= 0) && (point_x < SIZE_ROW) && (point_y >= 0) && (point_y < SIZE_COL) && (sol[point_x][point_y] == 0));
}

static size_t CountValidMoves(int sol[SIZE_ROW][SIZE_COL], int knights_move[][NUM_OF_AX], int point_x, int point_y)
{
    size_t i = 0;
    size_t count = 0;

    for (i = 0; MOVES > i; ++i)
    {
        count += MoveIsValid(sol, point_x + knights_move[i][X_AXIS], point_y + knights_move[i][Y_AXIS]);
    }

    return count;
}

static void InitKnightsMoves(int knights_moves[][NUM_OF_AX])
{
    knights_moves[0][X_AXIS] = -2;
    knights_moves[0][Y_AXIS] = -1;

    knights_moves[1][X_AXIS] = -1;
    knights_moves[1][Y_AXIS] = -2;

    knights_moves[2][X_AXIS] = -2;
    knights_moves[2][Y_AXIS] = 1;
    
    knights_moves[3][X_AXIS] = -1;
    knights_moves[3][Y_AXIS] = 2;

    knights_moves[4][X_AXIS] = 1;
    knights_moves[4][Y_AXIS] = -2;
    
    knights_moves[5][X_AXIS] = 1;
    knights_moves[5][Y_AXIS] = 2;
        
    knights_moves[6][X_AXIS] = 2;
    knights_moves[6][Y_AXIS] = 1;
        
    knights_moves[7][X_AXIS] = 2;
    knights_moves[7][Y_AXIS] = -1;
}

static void ClearBoard(int matrix[SIZE_ROW][SIZE_COL])
{
    size_t i = 0, j = 0;

    for(i = 0; i < SIZE_ROW; ++i)
    {
        for (j = 0; j < SIZE_COL; ++j)
        {
            matrix[i][j] = 0;
        }
    }
}

static int MinHeapCmp(const void *lhs, const void *rhs)
{
	options_t *moves_l = (options_t *)lhs; 
	options_t *moves_r = (options_t *)rhs; 

    return (moves_r->options - moves_l->options);
}