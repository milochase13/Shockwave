#include <stdlib.h>
#include <stdio.h>
#include "pos.h"

pos make_pos(unsigned int r, unsigned int c){
    pos* p = (pos*)malloc(sizeof(pos));
    p->r = r;
    p->c = c;
    return *p;
}
