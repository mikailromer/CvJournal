#ifndef POSITION_H
#define POSITION_H

class Position
{
private:
    char position_name[100];
public:
    char* get_position();
    void set_position_name(const char* name);
    void print_position_name(int pos_name);
    int clean_position();
};

#endif