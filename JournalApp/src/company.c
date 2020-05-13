#include "company.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>

static int get_num_of_positions(FILE* cv_list, uint* num_of_pos, int point_pos,
        bool getFromCvList, bool isCompanyNew);
static int add_new_positions(Position** Positions, uint32_t* num_of_pos, int num_of_new_pos);
static void set_positions_list(FILE* cv_list,Position** Positions, int num_of_pos, int point_pos);
static void set_name(char* company_name, const char* name);
static void print_positions(Position* Positions, uint num_of_pos);
static void save_new_positions_in_cv_list(FILE* cv_list, Position* Positions, uint num_of_pos);
static void clean_company_data(Position** Positions, uint* num_of_pos, char* company_name);

int init_companies(Company** Companies, uint num_of_comps,
        uint num_of_new_comps)
{
    uint i, begin, end;
    if(num_of_comps == 0) *Companies = NULL;
    *Companies = (Company*) realloc(*Companies, sizeof(Company) *
        (num_of_comps + num_of_new_comps));

    if(*Companies == NULL)
    {
        printf("Memory reallocation for Companies class variable has failed.\n");
        return -1;
    }

    if(num_of_comps == 0)
    {
        begin = 0;
        end = num_of_new_comps;
    }
    else
    {
        begin = num_of_comps;
        end = num_of_comps + num_of_new_comps;
    }

    for(i = begin; i < end; i++)
    {
        (*Companies)[i].get_num_of_positions_ptr = get_num_of_positions;
        (*Companies)[i].add_new_positions_ptr = add_new_positions;
        (*Companies)[i].set_positions_list_ptr = set_positions_list;
        (*Companies)[i].set_name_ptr = set_name;
        (*Companies)[i].print_positions_ptr = print_positions;
        (*Companies)[i].save_new_positions_in_cv_list_ptr = save_new_positions_in_cv_list;
        (*Companies)[i].clean_company_data_ptr = clean_company_data;
        memset((*Companies)[i].company_name,'\0', sizeof((*Companies)[i].company_name));
        (*Companies)[i].num_of_positions = 0;
        (*Companies)[i].Positions = NULL;
    }
    return 0;
}

static int get_num_of_positions(FILE* cv_list, uint* num_of_pos, int point_pos,
        bool getFromCvList, bool isCompanyNew)
{
    if(getFromCvList)
    {
        *num_of_pos = 0;
        char row[100];
        for (;;)
        {
            fgets(row, sizeof(row), cv_list);
            if (strstr(row, "Position: ")) (*num_of_pos)++;
            if (strstr(row, "Date: ") || strstr(row,"Company: ") || feof(cv_list)) break;
            memset(row,'\0', sizeof(row));
        }
        fseek(cv_list, point_pos, SEEK_SET);
    }
    else if(!getFromCvList && isCompanyNew) *num_of_pos = 0;

    return *num_of_pos;
}

static int add_new_positions(Position** Positions, uint32_t* num_of_pos, int num_of_new_pos)
{
    char row[100];
    uint i;
    init_positions(Positions, *num_of_pos, num_of_new_pos);
    for(i = *num_of_pos; i < *num_of_pos + num_of_new_pos; i++)
    {
        printf("\nYou need to give %i positions.\n", num_of_new_pos);
        printf("Type the name of the position No.%i: ", i+1);
        fgets(row, 100, stdin);
        (*Positions)[i].set_position_name_ptr((*Positions)[i].position_name, row);
        memset(row, '\0', sizeof(row));
    }

    *num_of_pos += num_of_new_pos;
    return 0;
}

static void set_positions_list(FILE* cv_list,Position** Positions, int num_of_pos, int point_pos)
{
    char row[100];
    char* p;
    int position_num = -1;
    init_positions(Positions, 0, num_of_pos );
    for (;;)
    {
        fgets(row, sizeof(row), cv_list);
        p = strstr(row, "Position: ");
        if (p)
        {
            p += 10;
            position_num++;
            (*Positions)[position_num].set_position_name_ptr
                    ((*Positions)[position_num].position_name, p);
        }
        if (strstr(row, "Date: ") || strstr(row, "Company: ") || feof(cv_list)) break;
        memset(row,'\0',sizeof(row));
    }

    fseek(cv_list, point_pos, SEEK_SET);
}


static void set_name(char* company_name, const char* name)
{
    strcpy(company_name, name);
    company_name[strlen(company_name) - 1] = '\0';
}

static void print_positions(Position* Positions, uint num_of_pos)
{
    uint i;
    for(i = 0; i < num_of_pos; i++)
    {
        printf("\t\tPosition No.%i: %s\n", i + 1, Positions[i].position_name);
    }
}

static void save_new_positions_in_cv_list(FILE* cv_list, Position* Positions, uint num_of_pos)
{
    uint i;
    for(i = 0; i < num_of_pos; i++)
    {
        fprintf(cv_list, "\n\t\tPosition: %s",Positions[i].position_name);
    }
    fprintf(cv_list, "\n");
}

static void clean_company_data(Position** Positions, uint* num_of_pos, char* company_name)
{
    uint i;
    for(i = 0; i < *num_of_pos; i++)
    {
       memset((*Positions)[i].position_name, '\0', sizeof((*Positions)[i].position_name));
    }
    free(*Positions);

    *Positions = NULL;
    *num_of_pos = NULL;
    memset(company_name,'\0', sizeof(company_name));
}

