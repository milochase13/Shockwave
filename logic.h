#ifndef LOGIC_H
#define LOGIC_H

#include "board.h"


enum turn {
    BLACKS_TURN,
    WHITES_TURN
};

typedef enum turn turn;


enum outcome {
    IN_PROGRESS,
    BLACK_WIN,
    WHITE_WIN,
    DRAW
};

typedef enum outcome outcome;


enum direction {
    N, E, S, W
};

typedef enum direction direction;


struct game {
    unsigned int run;
    board* b;
    turn player;
};

typedef struct game game;

/* new_game: given a run value, a width, a height, and a game type, returns a 
 * new game with a new board.*/
game* new_game(unsigned int run, unsigned int width, unsigned int height,
               enum type type);

/* game_free: frees a given game from memory*/
void game_free(game* g);

/* place_piece: places the piece of the player whose turn it is at the given 
 * position in the given game*/
int place_piece(game* g, pos p);

/* shockwave: moves piece towards the direction specified. pieces stop when 
 * they hit another piece or the side of the board. */
void shockwave(game* g, direction d);

/* outcome: given a game, returns the state of the game. that is, if black has
 * won, returns black_win, if white has won, returns white_win, if its a draw
 * returns draw, otherwise it returns in_progress.*/
outcome game_outcome(game* g);

#endif /* LOGIC_H */
