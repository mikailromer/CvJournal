#include "position.h"
#include <cstdlib>
#include <cstring>

char* Position::get_position()
{
    return position_name;
}

void Position::set_position_name(const char* name)
{
    strcpy(position_name, name);
    position_name[strlen(position_name) - 1] = '\0';
}

int Position::clean_position()
{
    memset(position_name, '\0', sizeof(position_name));
    return 0;
}