#include "company.h"
#include <cstdlib>
#include <cstring>
#include <cstdint>

Company::Company()
{
    Positions = NULL;
}

int Company::get_num_of_positions(FILE* cv_list, int point_pos, bool getFromCvList)
{
    num_of_positions = 0;
    if(getFromCvList)
    {
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

    return num_of_positions;
}

int Company::add_new_positions(int num_of_new_pos)
{
    char row[100];
    Positions = (Position*) realloc(Positions, sizeof(Position) *
            (num_of_positions + num_of_new_pos));
    if(!Positions)
    {
        printf("Memory allocation for Positions class variable has failed.\n");
        return -1;
    }

    for(uint i = num_of_positions; i < num_of_positions + num_of_new_pos; i++)
    {
        printf("\nYou need to give %i positions.\n", num_of_new_pos);
        printf("Type the name of the position No.%i: ", i+1);
        fgets(row, 100, stdin);
        Positions[i].set_position_name(row);
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
    Positions = (Position*) malloc(sizeof(Position) * num_of_positions);
    for (;;)
    {
        fgets(row, sizeof(row), cv_list);
        p = strstr(row, "Position: ");
        if (p)
        {
            p += 10;
            position_num++;
            Positions[position_num].set_position_name(p);
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
        printf("\t\tPosition No.%i: %s\n", i + 1, Positions[i].get_position());
    }
}

void Company::save_new_positions_in_cv_list(FILE* cv_list)
{
    for(uint i = 0; i < num_of_positions; i++)
    {
        fprintf(cv_list, "\n\t\tPosition: %s",Positions[i].get_position());
    }
    fprintf(cv_list, "\n");
}

void Company::clean_company_data()
{
    for(uint i = 0; i < num_of_positions; i++)
    {
        Positions[i].clean_position();
    }
    free(Positions);

    Positions = NULL;
    num_of_positions = NULL;
    memset(company_name,'\0', sizeof(company_name));
}