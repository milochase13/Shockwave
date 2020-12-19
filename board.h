#ifndef BOARD_H
#define BOARD_H

#include "pos.h"


enum cell {
    EMPTY,
    BLACK,
    WHITE
};

typedef enum cell cell;


union board_rep {
    enum cell** matrix;
    unsigned int* bits;
};

typedef union board_rep board_rep;

enum type {
    MATRIX, BITS
};


struct board {
    int width, height;
    enum type type;
    board_rep u;
};

typedef struct board board;

/* board_new: creates a new board of specified width, height, and type) */
board* board_new(unsigned int width, unsigned int height, enum type type);

/* board_free: frees given board in memory*/
void board_free(board* b);

/* board_show: prints out a ascii representation of the given board, complete 
 * with numbered rows and columns */
void board_show(board* b);

/* board_get: gets the cell value of the given board at given position.*/
cell board_get(board* b, pos p);

/* board_set: sets a given position in a given board to a given value. */
void board_set(board* b, pos p, cell c);

#endif /* BOARD_H */
