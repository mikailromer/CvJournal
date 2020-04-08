#ifndef COMPANY_H
#define COMPANY_H
#include "position.h"
#include <cstdio>
#include <cstdint>

class Company
{
private:
    char company_name[100];
    Position* Positions;
    uint32_t num_of_positions;

public:
    Company();
    ~Company();
    char* get_name();
    void set_name(const char *name);
    int get_num_of_positions(FILE* cv_list, int point_pos,
            bool getFromCvList, bool isCompanyNew);
    int add_new_positions(int num_of_pos);
    void set_positions_list(FILE* cv_list, int point_pos);
    void print_positions();
    void save_new_positions_in_cv_list(FILE* cv_list);
    void clean_company_data();
};

#endif