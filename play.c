#include <stdlib.h>
#include <stdio.h>
#include "pos.h"
#include "board.h"
#include "logic.h"
#include <string.h>

/*prompt: helper for main. prints current player's turn.*/ 
void prompt(game* g){
    switch(g->player){
        case BLACKS_TURN: printf("\nblack: ");
            break;
        case WHITES_TURN: printf("\nwhite: ");
            break;
    }
}

/* d: helper function. given a char, returns a direction if it is N,S,E, or W */
enum direction d(char c){
    enum direction dir;
    switch(c){
        case 78: dir = N;
            break;
        case 83: dir = S;
            break;
        case 69: dir = E;
            break;
        default: dir = W;
    }
    return dir;
}

/* range: helper function. checks if char is within range of numbers or 
 * letters.*/
int range(char c){
    if((48 <= c && c <= 57) || (90 >= c && c >= 65) || (122 >= c && c >= 97)){
        return 1;
    }
    else{
        return 0;
    }
}

/* check_in: helper function. checks if two given chars are within range or if
 * first char is = ! char.*/
int check_in(char c1, char c2){
    if(c1 == 33 && range(c2)){
        return -1;
    }
    else if(range(c1)){
        return range(c2);
    }
    else{
        return 0;
    }
}

/* char_to_num: helper function. turns given char into a position value 
 * unsigned int.*/
unsigned int char_to_num(char c){
    int n = c - 48;
    if(n < 10){
        return n;
    }
    else if(n >= 17 && n <= 42){
        n -= 7;
        return n;
    }
    else if(n >= 97&& n <= 122){
        return n -= 63;
    }
    return -1;
}

/* input: helper function. handles the process of taking a command for game
 * play.*/
int input(game* g){
    char c1, c2;
    scanf("%c%c%*c", &c1, &c2);
    int check = check_in(c1, c2);
    int nc1, nc2;
    nc1 = char_to_num(c1);
    nc2 = char_to_num(c2);
    if(check == 1 && nc1 >= 0 && nc1 <= g->b->height && nc2 >= 0 && nc2 <= 
        g->b->width){
        return place_piece(g, make_pos(nc1, nc2));
    }
    else if(check == -1 && (c2 == 78 || c2 == 83 || c2 == 69 || c2 == 87)){
        shockwave(g, d(c2));
        return 1;
    }
    else{
        printf("\ninput cannot be parsed!");
        return 0;
    }
}

/* g_state: helper function prints state of given game and returns 
 * pseudoboolean true if game is ongoing, false otherwise.*/
int g_state(game* g){
    switch (game_outcome(g)){
        case BLACK_WIN: 
            board_show(g->b);
            printf("\nblack wins!\n");
            return 0;
        case WHITE_WIN:        
            board_show(g->b);
            printf("\nwhite wins!\n");
            return 0;
        case DRAW:        
            board_show(g->b);
            printf("\ndraw!\n");
            return 0;
        default:
            board_show(g->b);
            return 1;
    }
}

/* main: run the process of a game.*/
int main(int argc, char *argv[]){
    unsigned int r = 0, h = 0, w = 0, stop = 1;
    enum type mb;
    for(int i = 1; i < argc; i++){
        if (strcmp(argv[i], "-r") == 0){ 
            r = atoi(argv[i+1]); 
        }
        else if (strcmp(argv[i], "-h") == 0){
            h = atoi(argv[i+1]);
        }
        else if(strcmp(argv[i], "-m") == 0 && stop){
            mb = MATRIX;
            stop = 0;
        }
        else if(strcmp(argv[i], "-b") == 0 && stop){
            mb = BITS;
            stop = 0;
        }
        else if (strcmp(argv[i], "-w") == 0){
            w = atoi(argv[i+1]);
        }
    }
    game* g = new_game(r, w, h, mb);
    int state = 1;
    board_show(g->b);
    while(state){
        prompt(g);
        int i = input(g);
        if(i){
            state = g_state(g);
        }
        else{
            continue;
        }
    }
    game_free(g);
    return 0;
}
