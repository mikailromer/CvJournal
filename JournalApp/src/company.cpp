#include "company.h"
#include <cstdlib>
#include <cstring>
#include <cstdint>

int Company::get_num_of_positions(FILE* cv_list, int point_pos,
        bool getFromCvList, bool isCompanyNew)
{
    if(getFromCvList)
    {
        num_of_positions = 0;
        char row[100];
        for (;;)
        {
            fgets(row, sizeof(row), cv_list);
            if (strstr(row, "Position: ")) num_of_positions++;
            if (strstr(row, "Date: ") || strstr(row,"Company: ") || feof(cv_list)) break;
            memset(row,'\0', sizeof(row));
        }
        fseek(cv_list, point_pos, SEEK_SET);
    }
    else if(!getFromCvList && isCompanyNew) num_of_positions = 0;

    return num_of_positions;
}

int Company::add_new_positions(int num_of_new_pos)
{
    char row[100];
    init_positions(&Positions,num_of_positions, num_of_new_pos);
    for(uint i = num_of_positions; i < num_of_positions + num_of_new_pos; i++)
    {
        printf("\nYou need to give %i positions.\n", num_of_new_pos);
        printf("Type the name of the position No.%i: ", i+1);
        fgets(row, 100, stdin);
        Positions[i].set_position_name_ptr(Positions[i].position_name, row);
        memset(row, '\0', sizeof(row));
    }

    num_of_positions += num_of_new_pos;
    return 0;
}

void Company::set_positions_list(FILE* cv_list, int point_pos)
{
    char row[100];
    char* p;
    int position_num = -1;
    init_positions(&Positions, 0, num_of_positions );
    for (;;)
    {
        fgets(row, sizeof(row), cv_list);
        p = strstr(row, "Position: ");
        if (p)
        {
            p += 10;
            position_num++;
            Positions[position_num].set_position_name_ptr(Positions[position_num].position_name, p);
        }
        if (strstr(row, "Date: ") || strstr(row, "Company: ") || feof(cv_list)) break;
        memset(row,'\0',sizeof(row));
    }

    fseek(cv_list, point_pos, SEEK_SET);
}

void Company::set_name(const char* name)
{
    strcpy(company_name, name);
    company_name[strlen(company_name) - 1] = '\0';
}

char* Company::get_name()
{
    return company_name;
}

void Company::print_positions()
{
    for(uint i = 0; i < num_of_positions; i++)
    {
        printf("\t\tPosition No.%i: %s\n", i + 1, Positions[i].position_name);
    }
}

void Company::save_new_positions_in_cv_list(FILE* cv_list)
{
    for(uint i = 0; i < num_of_positions; i++)
    {
        fprintf(cv_list, "\n\t\tPosition: %s",Positions[i].position_name);
    }
    fprintf(cv_list, "\n");
}

void Company::clean_company_data()
{
    for(uint i = 0; i < num_of_positions; i++)
    {
    	memset(Positions[i].position_name, '\0', sizeof(Positions[i].position_name));
    }
    free(Positions);

    Positions = NULL;
    num_of_positions = NULL;
    memset(company_name,'\0', sizeof(company_name));
}
