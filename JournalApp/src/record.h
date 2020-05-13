#ifndef RECORD_H
#define RECORD_H
#include "company.h"
#include <cstdint>
#include <cstdio>

typedef struct
{
    uint8_t day;
    uint8_t month;
    uint32_t year;
    Company *Companies;
    uint32_t num_of_companies;
    int (*get_num_of_companies_ptr)(FILE* ,uint32_t* ,int ,bool ,bool );
    int (*set_list_of_companies_ptr)(FILE* ,Company** ,uint32_t* ,int );
    int (*add_new_companies_ptr)(Company** ,uint32_t* ,int );
    int (*update_company_ptr)(Company** ,int );
    void (*print_date_ptr)(uint8_t ,uint8_t ,uint32_t ,int );
    void (*set_date_ptr)(uint8_t* ,uint8_t* ,uint32_t* ,int* );
    void (*print_companies_ptr)(Company* ,uint32_t );
    void (*save_new_companies_in_cv_list_ptr)(FILE* ,Company** ,uint32_t );
    void (*clean_record_data_ptr)(Company** , uint32_t*,
    		uint8_t* ,uint8_t* ,uint32_t* );
}Record;

int init_records(Record** Records, uint num_of_recs,
		uint num_of_new_recs);

#endif // !RECORD
