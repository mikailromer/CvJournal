#include "record.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

Record::Record()
{
    Companies = NULL;
}

void Record::clean_record_data()
{
    for(int i = 0; i < num_of_companies; i++)
    {
        Companies[i].clean_company_data();
    }
    free(Companies);

    Companies = NULL;
    num_of_companies = NULL;
    day = NULL;
    month = NULL;
    year = NULL;
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
    num_of_companies = 0;
    if(getFromCvList)
    {
        char row[100];
        for (;;)
        {
            fgets(row, sizeof(row), cv_list);
            if (strstr(row, "Company:"))num_of_companies++;
            if (strstr(row, "Date: ") || feof(cv_list)) break;
            memset(row,'\0', sizeof(row));
        }

        fseek(cv_list, point_pos, SEEK_SET);
    }

    return num_of_companies;
}

int Record::set_list_of_companies(FILE *cv_list, int point_pos)
{
    char row[100];
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
        memset(row,'\0',sizeof(row));
    }
    fseek(cv_list, point_pos, SEEK_SET);

    return 0;
}

int Record::add_new_companies(int num_of_new_comps)
{
    Companies = (Company*) realloc(Companies, sizeof(Company) * 
    (num_of_companies + num_of_new_comps));

    if(!Companies)
    {
        printf("Memory allocation for Companies class variable has failed.\n");
        return -1;
    }
    char row[100];
    char choice;
    int num_of_new_pos;
    for(int i = num_of_companies; i < num_of_companies + num_of_new_comps; i++)
    {
       // system("clear");
        printf("\nNumber of companies added to the record: %i.\n",
                num_of_companies + num_of_new_comps);
        printf("Type the name of Company No.%i: ", i+1);
        fgets(row, 100, stdin);
        Companies[i].set_name(row);
        num_of_new_pos = 0;
        memset(row,'\0', sizeof(row));
        do
        {
            //system("clear");
            printf("\nYou need to add at least one job position.\n");
            printf("Number of positions assigned to the company: %i.\n",
                    num_of_new_pos);
            printf("Do you want to add a new job position [Y/N]? ");
            choice = getc(stdin);
            getchar();
            if(choice == 'Y') num_of_new_pos++;
        } while(choice != 'N' || num_of_new_pos == 0);
        Companies[i].get_num_of_positions(NULL, 0, false);
        Companies[i].add_new_positions(num_of_new_pos);
    }

    num_of_companies+= num_of_new_comps;
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
        printf("\tCompany No.%i: %s\n",i + 1, Companies[i].get_name());
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