#ifndef POSITION_H
#define POSITION_H
#include <cstdlib>

typedef struct
{
	char position_name[100];
	void (*set_position_name_ptr)(char*, const char*);
}Position;

int init_positions(Position** Positions, uint num_of_pos,
		uint num_of_new_pos);
#endif
