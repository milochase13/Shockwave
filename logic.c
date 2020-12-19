#include <stdlib.h>
#include <stdio.h>
#include "logic.h"

game* new_game(unsigned int run, unsigned int width, unsigned int height,
               enum type type){
    if(run > width && run > height){
        fprintf(stderr, "new_game: cannot require run larger than board\n");
        exit(1);
    }
    game* g = (game*)malloc(sizeof(game));
    g->run = run;
    g->b = board_new(width, height, type);
    g->player = BLACKS_TURN;
    return g;
}

void game_free(game* g){
    board_free(g->b);
    free(g);
}

int place_piece(game* g, pos p){
    if(p.r >= g->b->height || p.c >= g->b->width){
        fprintf(stderr, "place_piece: cannot place piece outside board.\n");
        exit(1);
    }
    switch(board_get(g->b, p)){
        case EMPTY:
            switch(g->player){
                case BLACKS_TURN:            
                    board_set(g->b, p, BLACK);
                    g->player = WHITES_TURN;
                    return 1;
                case WHITES_TURN:
                    board_set(g->b, p, WHITE);
                    g->player = BLACKS_TURN;
                    return 1;
                default:    
                    return 0;
            }
        default:
            return 0;
    }
}

/* make_moves: helper for shockwave. given a board, a vertical value 
 * (1, 0, or -1) a horizontal value (1, 0, -1), an initial y value, and 
 * initial x value, and an index value, this function makes the necessary 
 * moves for the given index in the necessary direction in accordence with 
 * the requirements for "shockwave"*/
void make_moves(board* b, int v, int h, int val, int cnst, int x){
    int i = x;
    if(v == 1){
        while(i + 1 < b->height && board_get(b, make_pos(i + 1, cnst)) == EMPTY){
            i++;
        }
        board_set(b, make_pos(i, cnst), board_get(b, make_pos(x, cnst)));
        if(x != i){
            board_set(b, make_pos(x, cnst), EMPTY);
        }
    }
    else if(v == -1){
        while(i - 1 >= 0 && board_get(b, make_pos(i - 1, cnst)) == EMPTY){
            i--;
        }
        board_set(b, make_pos(i, cnst), board_get(b, make_pos(val, cnst)));
        if(val != i){
            board_set(b, make_pos(val, cnst), EMPTY);
        }
    }   
    else if(h == 1){
        while(i + 1 < b->width && board_get(b, make_pos(cnst, i + 1)) == EMPTY){
            i++;
        }
        board_set(b, make_pos(cnst, i), board_get(b, make_pos(cnst, x)));
        if(x != i){
            board_set(b, make_pos(cnst, x), EMPTY);
        }
    }
    else{
        while(i - 1 >= 0 && board_get(b, make_pos(cnst, i - 1)) == EMPTY){
            i--;
        }
        board_set(b, make_pos(cnst, i), board_get(b, make_pos(cnst, val)));
        if(val != i){
            board_set(b, make_pos(cnst, val), EMPTY);
        }
    }
}

/* move: helper for shockwave. calculates the correct starting position for
 * analysis.then, iterates through given board and calls the make_moves 
 * function to do the rest.*/
void move (board* b, int v, int h, enum cell player){
    for (int r = 0; r < b->height; r++) {
        for (int c = 0; c < b->width; c++) {
            int x = 0;
            int y = 0;
            if(v == 1){
                x = b->height - 1 - r;
            }
            else{
                x = r;
            }
            if(h == 1){
                y = b->width - 1 - c;
            }
            else{
                y = c;
            }   
            if (board_get(b, make_pos(x, y)) == player){
                if(v != 0){
                    make_moves(b, v, h, r, c, x);
                }
                else{
                    make_moves(b, v, h, c, r, y);
                }
            }
        }
    }
}

void shockwave(game* g, direction d){
    enum cell player;
    switch(g->player){
        case BLACKS_TURN:
            player = BLACK;
            g->player = WHITES_TURN;
            break;
        default:
            player = WHITE;
            g->player = BLACKS_TURN;
    }
    switch(d){
        case N:
            move(g->b, -1, 0, player);
            break;
        case S:
            move(g->b, 1, 0, player);
            break;
        case W:
            move(g->b, 0, -1, player);
            break;
        case E:
            move(g->b, 0, 1, player);
            break;
    }
}

/* check_rows: helper for outcome. checks all rows for a run. returns the cell
 * type of the run if found. otherwise return empty. if there is a draw, the
 * draw out parameter is 1. */
enum cell check_rows(game* g, int* draw){
    board* b = g->b;
    enum cell player = EMPTY;
    *draw = 1;
    for(int i = 0; i < b->height; i++){
        int count = 0;
        for(int j = 0; j < b->width; j++){
            enum cell p_temp = board_get(b, make_pos(i, j));
            if(player != EMPTY && player == p_temp){
                count++;
            }
            else if(player == EMPTY && board_get(b, make_pos(i, j)) != EMPTY){
                count++;
                player = p_temp;
            }
            else if(board_get(b, make_pos(i, j)) != EMPTY){
                count = 1; 
                player = p_temp;
            }
            else{
                count = 0;
                player = EMPTY;
                *draw = 0;
            }
            if(count >= g->run){
                return player;
            }
        }
    }
    return EMPTY;
}

/* check_cols: helper for outcome. checks all cols for a run. returns the cell
 * type of the run if found. otherwise return empty.*/
enum cell check_cols(game* g){
    board* b = g->b;
    enum cell player = EMPTY;
    for(int i = 0; i < b->width; i++){
        int count = 0;
        for(int j = 0; j < b->height; j++){
            enum cell p_temp = board_get(b, make_pos(j, i));
            if(player != EMPTY && player == p_temp){
                count++;
            }
            else if(player == EMPTY && board_get(b, make_pos(j, i)) != EMPTY){
                count++;
                player = p_temp;
            }
            else if(board_get(b, make_pos(j, i)) != EMPTY){
                count = 1;
                player = p_temp;
            }
            else{
                count = 0;
                player = EMPTY;
            }
            if(count >= g->run){
                return player;
            }
        }
    }
    return EMPTY;
}

/* check_diag_up: helper for outcome. checks all increasing diagonals for a 
 * run. returns the cell type of the run if found. otherwise return empty.*/
enum cell check_diag_up(game* g){
    board* b = g->b;
    enum cell player = EMPTY;
    for(int i = 0; i < b->height; i++){
        int j = 0;
        int count = 0;
        int k = i;
        while(j < b->width && k >= 0){
            enum cell p_temp = board_get(b, make_pos(k, j));
            if(player != EMPTY && player == p_temp){
                count++;
            }
            else if(player == EMPTY && board_get(b, make_pos(k, j)) != EMPTY){
                count++;
                player = p_temp;
            }
            else if(board_get(b, make_pos(k, j)) != EMPTY){
                count = 1;
                player = p_temp;
            }
            else{
                count = 0;
                player = EMPTY;
            }
            if(count >= g->run){
                return player;
            }
            j++;
            k--;
        }
    }
    for(int i = 0; i < b->height; i++){
        int k = b->height - 1;
        int count = 0;
        int j = i;
        while(j < b->width && k >= 0){
            enum cell p_temp = board_get(b, make_pos(k, j));
            if(player != EMPTY && player == p_temp){
                count++; 
            }   
            else if(player == EMPTY && board_get(b, make_pos(k, j)) != EMPTY){
                count++;
                player = p_temp;
            }   
            else if(board_get(b, make_pos(k, j)) != EMPTY){
                count = 1;
                player = p_temp;
            }   
            else{
                count = 0;
                player = EMPTY;
            }   
            if(count >= g->run){
                return player;
            }   
            j++;
            k--;
        }   
    }
    return EMPTY;
}

/* check_diag_down: helper for outcome. checks all decreasing diagonals for a 
 * run. returns the cell type of the run if found. otherwise return empty.*/
enum cell check_diag_down(game* g){
    board* b = g->b;
    enum cell player = EMPTY;
    for(int i = 0; i < b->height; i++){
        int j = 0;
        int count = 0;
        int k = i;
        while(j < b->width && k < b->height){
            enum cell p_temp = board_get(b, make_pos(k, j));
            if(player != EMPTY && player == p_temp){
                count++;
            }
            else if(player == EMPTY && board_get(b, make_pos(k, j)) != EMPTY){
                count++;
                player = p_temp;
            }
            else if(board_get(b, make_pos(k, j)) != EMPTY){
                count = 1;
                player = p_temp;
            }
            else{
                count = 0;
                player = EMPTY;
            }
            if(count >= g->run){
                return player;
            }
            j++;
            k++;
        }
    }
    for(int i = 0; i < b->height; i++){
        int k = 0;
        int count = 0;
        int j = i;
        while(j < b->width && k < b->height){
            enum cell p_temp = board_get(b, make_pos(k, j));
            if(player != EMPTY && player == p_temp){
                count++;
            }
            else if(player == EMPTY && board_get(b, make_pos(k, j)) != EMPTY){
                count++;
                player = p_temp;
            }
            else if(board_get(b, make_pos(k, j)) != EMPTY){
                count = 1;
                player = p_temp;
            }
            else{
                count = 0;
                player = EMPTY;
            }
            if(count >= g->run){
                return player;
            }
            j++;
            k++;
        }
    }
    return EMPTY;
}

/* convert_to_outcome: helper for outcome. given a cell, converts to 
 * appropriate outcome. */
outcome convert_to_outcome(enum cell p){
    switch(p){
        case BLACK:
            return BLACK_WIN;
            break;
        case WHITE:
            return WHITE_WIN;
            break;
        default:
            return IN_PROGRESS;
    }
}

outcome game_outcome(game* g){
    int drw = 0;
    enum cell r = check_rows(g, &drw);
    enum cell c = check_cols(g);
    enum cell du = check_diag_up(g);
    enum cell dd = check_diag_down(g);
    if(r != EMPTY){
        return convert_to_outcome(r);
    }   
    else if(c != EMPTY){
        return convert_to_outcome(c);
    }
    else if(du != EMPTY){
        return convert_to_outcome(du);
    }
    else if(dd != EMPTY){
        return convert_to_outcome(dd);
    }
    else if(drw == 1){
        return DRAW;
    }
    else{
        return IN_PROGRESS;
    }
}
