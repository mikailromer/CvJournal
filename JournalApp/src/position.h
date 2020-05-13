#ifndef POSITION_H
#define POSITION_H
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

typedef struct
{
    char position_name[100];
    void (*set_position_name_ptr)(char*, const char*);
}Position;

int init_positions(Position** Positions, unsigned int num_of_pos,
        unsigned int num_of_new_pos);
#endif
