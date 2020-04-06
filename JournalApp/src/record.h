#ifndef RECORD_H
#define RECORD_H
#include "company.h"
#include <cstdint>
#include <cstdio>

class Record
{
public:
    Record();
    int get_day();
    int get_month();
    int get_year();
    int get_num_of_companies(FILE* cv_list, int point_pos, bool getFromCvList);
    int set_list_of_companies(FILE* cv_list, int point_pos);
    int add_new_companies(int num_of_comps);

    void set_date(int* date);
    void print_date(int record_num);
    void print_companies();
    void save_new_companies_in_cv_list(FILE* cv_list);
    void clean_record_data();

private:
    uint8_t day;
    uint8_t month;
    uint32_t year;
    Company *Companies;
    int num_of_companies;
};

#endif // !RECORD
