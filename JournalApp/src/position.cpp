#include "position.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

int init_positions(Position** Positions,int num_of_pos,
		int num_of_new_pos, bool isPositionTableExist)
{
	int i, begin, end;
	if(!isPositionTableExist) *Positions = NULL;
	*Positions = (Position*)realloc(Positions,sizeof(Position) *
			(num_of_pos + num_of_new_pos));

	if(*Positions)
	{
		printf("Memory reallocation for Positions struct variable has failed.\n");
		return -1;
	}

	if(!isPositionTableExist)
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
		memset((*Positions)->position_name,'\0', sizeof((*Positions)->position_name));
	}

	return 0;
}

/*

static char* get_position()
{
    return position_name;
}

static void set_position_name(const char* name)
{
    strcpy(position_name, name);
    if( position_name[strlen(position_name) - 1] == '\n')
    {
        position_name[strlen(position_name) - 1] = '\0';
    }
}

static int clean_position()
{
    memset(position_name, '\0', sizeof(position_name));
    return 0;
}
*/
