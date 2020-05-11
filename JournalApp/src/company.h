#ifndef COMPANY_H
#define COMPANY_H
#include "position.h"
#include <cstdio>
#include <cstdlib>
#include <cstdint>

typedef struct
{
    char company_name[100];
    Position* Positions;
    uint32_t num_of_positions;
    int (*get_num_of_positions_ptr)(FILE* ,int* ,int ,bool ,bool );
    int (*add_new_positions_ptr)(Position** ,uint32_t* ,int );
    void (*set_positions_list_ptr)(FILE* ,Position** ,int ,int );
    void (*set_name_ptr)(char*, const char* );
    void (*print_positions_ptr)(Position* , int );
    void (*save_new_positions_in_cv_list_ptr)(FILE* , Position* , int );
    void (*clean_company_data_ptr)(Position** ,uint* ,char* );
}Company;

int init_companies(Company** Companies,int num_of_comps,
		int num_of_new_comps);
/*
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
*/
#endif
