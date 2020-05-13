#include "position.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

static void set_position_name(char* position_name, const char* name);

int init_positions(Position** Positions, uint num_of_pos,
        uint num_of_new_pos)
{
    uint i, begin, end;
    if(num_of_pos == 0) *Positions = NULL;
    *Positions = (Position*)realloc(*Positions,sizeof(Position) *
            (num_of_pos + num_of_new_pos));

    if(*Positions == NULL)
    {
        printf("Memory reallocation for Positions struct variable has failed.\n");
        return -1;
    }

    if(num_of_pos == 0)
    {
        begin = 0;
        end = num_of_new_pos;
    }
    else
    {
        begin = num_of_pos;
        end = num_of_pos + num_of_new_pos;
    }

    for(i = begin; i < end; i++)
    {
        (*Positions)[i].set_position_name_ptr = set_position_name;
        memset((*Positions)[i].position_name,'\0', sizeof((*Positions)[i].position_name));
    }

    return 0;
}

static void set_position_name(char* position_name, const char* name)
{
    strcpy(position_name, name);
    if( position_name[strlen(position_name) - 1] == '\n')
    {
        position_name[strlen(position_name) - 1] = '\0';
    }
}
