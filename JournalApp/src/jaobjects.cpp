#include "jaobjects.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

Record::Record()
{
    day = 0;
    month = 0;
    year = 0;
    num_of_companies = 0;
    Companies = NULL;
}

void Record::clean_record_data()
{
    for(int i = 0; i < num_of_companies; i++)
    {
        Companies[i].clean_company_data();
    }
    free(Companies);
}

int Record::get_day()
{
    return day;
}

int Record::get_month()
{
    return month;
}

int Record::get_year()
{
    return year;
}

int Record::get_num_of_companies(FILE* cv_list, int point_pos, bool getFromCvList)
{
    if(getFromCvList)
    {
        char row[100];
        int i = 0;
        for (;;)
        {
            fgets(row, sizeof(row), cv_list);
            if (strstr(row, "Company: "))num_of_companies++;
            if (strstr(row, "Date: ") || feof(cv_list)) break;
        }
        fseek(cv_list, point_pos, SEEK_SET);
    }

    return num_of_companies;
}

int Record::set_list_of_companies(FILE *cv_list, int point_pos)
{
    char row[100];
    int N;
    int comp_num = -1;
    char* p;
    Companies = (Company*) malloc(sizeof(Company) * num_of_companies);
    for (;;)
    {
        fgets(row, sizeof(row), cv_list);

        p = strstr(row, "Company: ");
        if (p)
        {
            p += 9;
            comp_num++;
            Companies[comp_num].set_name(p);
            Companies[comp_num].get_num_of_positions(cv_list, ftell(cv_list), true);
            Companies[comp_num].set_positions_list(cv_list, ftell(cv_list));
        }

        if (strstr(row, "Date: ") || feof(cv_list)) break;
    }
    fseek(cv_list, point_pos, SEEK_SET);

    return 0;
}

int Record::add_new_companies(int num_of_new_comps)
{   
    Companies = (Company*) realloc(Companies, sizeof(Company) * 
    (num_of_companies + num_of_new_comps));

    char row[100];
    char choice;
    int num_of_new_pos;
    for(int i = num_of_companies; i < num_of_companies + num_of_new_comps; i++)
    {
        system("cls");
        printf("Type the name of company: \n");
        fgets(row, 100, stdin);
        Companies[i].set_name(row);
        num_of_new_pos = 0;
        memset(row,'\0', sizeof(row));
        do
        {
            system("cls");
            printf("You need to add at least one job position. \n");
            printf("Do you want to add a new job position [Y/N]? \n");
            choice = getchar();
            if(choice == 'Y') num_of_new_pos++;
        } while(choice != 'N' && num_of_new_pos!=0);
        Companies[i].add_new_positions(num_of_new_pos);
    }

    return 0;
}

void Record::set_date(int* date)
{
    day = date[0];
    month = date[1];
    year = date[2];
}

void Record::print_date(int record_num)
{
    printf("\n");
    printf("Record No.%i\n",record_num);
    printf("Date: %i.%i.%i\n", day, month, year);
}

void Record::print_companies()
{
    printf("\n");
    for(int i = 0; i < num_of_companies; i++)
    {
        printf("\tCompany No.%i: %s\n",i, Companies[i].get_name());
        Companies[i].print_positions();
    }
}

void Record::save_new_companies_in_cv_list(FILE* cv_list)
{
    for(int i = 0; i< num_of_companies; i++)
    {
        fprintf(cv_list, "\n\tCompany: %s", Companies[i].get_name());
        Companies[i].save_new_positions_in_cv_list(cv_list);
    }
}

Company::Company()
{
    Positions = NULL;
    num_of_positions = 0;
}

int Company::get_num_of_positions(FILE* cv_list, int point_pos, bool getFromCvList)
{
    if(getFromCvList)
    {
        char row[100];
        int i = 0;
        for (;;)
        {
            fgets(row, sizeof(row), cv_list);
            if (strstr(row, "Position: ")) num_of_positions++;
            if (strstr(row, "Date: ") || strstr(row,"Company: ") || feof(cv_list)) break;
        }
        fseek(cv_list, point_pos, SEEK_SET);
    }

    return num_of_positions;
}

int Company::add_new_positions(int num_of_pos)
{
    char row[100];
    Positions = (Position*) realloc(Positions, sizeof(Position) * num_of_pos);
    for(int i = 0; i < num_of_pos; i++)
    {
        system("cls");
        printf("You need to give %i positions: \n", num_of_pos);
        printf("Type the name of the position No.%i: \n", i+1);
        fgets(row, 100, stdin);
        Positions[i].set_position_name(row);
        memset(row, '\0', sizeof(row));
    }

    return num_of_pos;
}

void Company::set_positions_list(FILE* cv_list, int point_pos)
{
    char row[100];
    char* p;
    int i = 0;
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
    for(int i = 0; i<num_of_positions;i++)
    {
        printf("\t\tPosition No.%i: %s\n",i, Positions[i].get_position());
    }
}

void Company::save_new_positions_in_cv_list(FILE* cv_list)
{
    for(int i = 0; i < num_of_positions; i++)
    {
        fprintf(cv_list, "\n\t\tPosition: %s",Positions[i].get_position());
    }
    fprintf(cv_list, "\n");
}

void Company::clean_company_data()
{
    free(Positions);
}

char* Position::get_position()
{
    return position_name;
}
void Position::set_position_name(const char* name)
{
    strcpy(position_name, name);
    position_name[strlen(position_name) - 1] = '\0';
}


