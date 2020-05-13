#ifndef COMPANY_H
#define COMPANY_H
#include "position.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"

typedef struct
{
    char company_name[100];
    Position* Positions;
    uint32_t num_of_positions;
    int (*get_num_of_positions_ptr)(FILE* ,unsigned int* ,int ,bool ,bool );
    int (*add_new_positions_ptr)(Position** ,uint32_t* ,int );
    void (*set_positions_list_ptr)(FILE* ,Position** ,int ,int );
    void (*set_name_ptr)(char*, const char* );
    void (*print_positions_ptr)(Position* , unsigned int );
    void (*save_new_positions_in_cv_list_ptr)(FILE* , Position* , unsigned int );
    void (*clean_company_data_ptr)(Position** ,unsigned int* ,char* );
}Company;

int init_companies(Company** Companies, unsigned int num_of_comps,
        unsigned int num_of_new_comps);

#endif
