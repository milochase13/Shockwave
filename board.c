#include <stdlib.h>
#include <stdio.h>
#include "board.h"
#include <math.h>

board* board_new(unsigned int width, unsigned int height, enum type type){
    if(type == BITS){
        unsigned int* a = (unsigned int*)malloc(width*height*2);
        for (int c = 0; c < width*height*2; c++) {
            a[c] = 0;
        }
        board* b = (board*)malloc(sizeof(board));
        b->width = width;
        b->height = height;
        b->type = type;
        b->u.bits = a;
        return b;
    }
    enum cell** a = (enum cell**)malloc(height * sizeof(enum cell*));
    for (int r = 0; r < height; r++) {
        a[r] = (enum cell*)malloc(width * sizeof(enum cell));
        for (int c = 0; c < width; c++) {
            a[r][c] = EMPTY;
        }
    }
    board* b = (board*)malloc(sizeof(board));
    b->width = width;
    b->height = height;
    b->type = type;
    b->u.matrix = a; 
    return b;
}

void board_free(board* b){
    if(b->type == BITS){
        free(b->u.bits);
        free(b);
    }
    else{
    for (int r = 0; r < b->height; r++) {
        free(b->u.matrix[r]);
    }
    free(b->u.matrix);
    free(b);
    }
}

/* print_token: helper for board_show. prints the appropriate symbol for
 * black, white, and empty spaces. */
void print_token(enum cell t){
    switch(t){
        case WHITE:
            printf("o");
            break;
        case BLACK:
            printf("*");
            break;
        default:
            printf(".");
    }
}

/* print_letters: helper for board_show. prints the appropriate letters for
 * values greater than 9 */
void print_letters(unsigned int r){
    char a = r + 55;
    if(a <= 90){
        printf("%c", a);
    }
    else if(r <= 61){
        a = r + 61;
        printf("%c", a);
    }
    else{
        printf("?");
    }
}

void print_bits(unsigned int b){
    unsigned int filter = 3;
    unsigned int c = b & filter;
    if(c == 1){
        printf("*"); 
    }
    else if(c == 2){
        printf("o");
    }
    else{
        printf(".");
    }
}   

void board_show(board* b){
    printf("  ");
    unsigned int w = b->width;
    for(unsigned int i = 0; i < w; i++){
        if(i>9){
            print_letters(i);
        }
        else{
            printf("%u",i);
        }
    }
    printf("\n\n");
    int j = 0;
    int line = 0;
    unsigned int bit_string;
    int shift = 0;
    for (unsigned int r = 0; r < b->height; r++) {
        if(r>9){
            print_letters(r);
            printf(" ");
        }
        else{
            printf("%u ",r);
        }
        switch(b->type){
        case MATRIX:
            for (unsigned int c = 0; c < b->width; c++) {
                print_token(b->u.matrix[r][c]);
            }
            printf("\n");
            break;
        case BITS:
            bit_string = b->u.bits[j] >> shift;
            for(unsigned int c = 0; c < b->width; c++) {
                if(c + r != 0 && (r * b->width + c)*2 % 
                  (sizeof(unsigned int)*8) == 0){
                    j++;
                    bit_string = b->u.bits[j];
                    shift = 0;
                }
                print_bits(bit_string);
                bit_string = bit_string >> 2;
                shift += 2;
                line++;
                if(line >= b->width){
                    printf("\n");
                    line = 0;
                }
            }
            break;
        }
    }
}

cell board_get(board* b, pos p){
    if(p.r >= b->height || p.c >= b->width || p.r < 0 || p.c < 0){
        fprintf(stderr, "board_get: cannot get pos outside board\n");
        exit(1);
    }
    int loc, remainder;
    unsigned int val;
    switch(b->type){
        case BITS:
            loc = (p.r * b->width + p.c)*2 / (sizeof(unsigned int) * 8);
            remainder = (p.r * b->width + p.c)*2 % (sizeof(unsigned int) * 8);
            val = b->u.bits[loc] >> remainder;
            if(val % 2 == 1){
                return BLACK;
            }
            else if((val >> 1) % 2 == 1){
                return WHITE;
            }
            else{
                return EMPTY;
            }
            break;
        default:
            return b->u.matrix[p.r][p.c];
    }
}

void board_set(board* b, pos p, cell c){
    if(p.r >= b->height || p.c >= b->width || p.r < 0 || p.c < 0){
        fprintf(stderr, "board_set: cannot get pos outside board\n");
        exit(1);
    }
    int loc, remainder;
    unsigned int filter;
    switch(b->type){
        case BITS:
            loc = (p.r * b->width + p.c )*2 / (sizeof(unsigned int) * 8);
            remainder = (p.r * b->width + p.c) * 2 % 
                        (sizeof(unsigned int) * 8);
            switch(c){
                case BLACK:
                    filter = pow(2, remainder);
                    break;
                case WHITE:
                    filter = pow(2, remainder + 1);
                    break;
                case EMPTY:
                    if(board_get(b, p) == WHITE){
                        b->u.bits[loc] -= pow(2, remainder + 1);
                    }
                    else if(board_get(b, p) == BLACK){
                        b->u.bits[loc] -= pow(2, remainder);
                    }
                    filter = 0;
                    break;
                }
            b->u.bits[loc] = b->u.bits[loc] | filter;
            break;
        default:
            b->u.matrix[p.r][p.c] = c;
    }
}

