#ifndef JAOBJECTS_H
#define JAOBJECTS_H
#include <cstdint>
#include <cstdio>

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

class Record
{
public:
    Record();
    int get_day();
    int get_month();
    int get_year();
    int get_num_of_companies(FILE* cv_list, int point_pos,
            bool getFromCvList, bool isRecordNew);
    int set_list_of_companies(FILE* cv_list, int point_pos);
    int add_new_companies(int num_of_comps);
    int update_company(int comp_num);

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

#endif // !JAOBJECTS
