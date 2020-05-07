#ifndef POSITION_H
#define POSITION_H

typedef struct
{
	char position_name[100];
	char* (*get_position_ptr)();
	void (*set_position_name_ptr)(const char* name);
	void (*print_position_name_ptr)(int pos_name);
	int (*clean_position_ptr)();
}Position;

int init_positions(Position** Positions,int num_of_pos,
		int num_of_new_pos, bool isPositionTableExist);
/*
static char* get_position();
static void set_position_name(const char* name);
static void print_position_name(int pos_name);
static int clean_position();
*/
#endif
