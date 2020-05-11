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
	Position* Positions;
	uint i;
    for(i = 0; i < num_of_companies; i++)
    {
    	Positions = Companies[i].Positions;
    	Companies[i].clean_company_data_ptr(&Positions,
    			&(Companies[i].num_of_positions),
    			Companies[i].company_name);
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

int Record::get_num_of_companies(FILE* cv_list, int point_pos,
        bool getFromCvList, bool isRecordNew)
{
    if(getFromCvList)
    {
        num_of_companies = 0;
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
    else if(!getFromCvList && isRecordNew) num_of_companies = 0;

    return num_of_companies;
}

int Record::set_list_of_companies(FILE *cv_list, int point_pos)
{
    char row[100];
    int comp_num = -1;
    char* p;
    //Companies = (Company*) malloc(sizeof(Company) * num_of_companies);

    init_companies(&Companies, 0, num_of_companies);
    for (;;)
    {
        fgets(row, sizeof(row), cv_list);

        p = strstr(row, "Company: ");
        if (p)
        {
            p += 9;
            comp_num++;
            Companies[comp_num].set_name_ptr(Companies[comp_num].company_name, p);
            //Companies[comp_num].set_name(p);
            Companies[comp_num].get_num_of_positions_ptr(cv_list,
            		&(Companies[comp_num].num_of_positions), ftell(cv_list), true, false);
            //Companies[comp_num].get_num_of_positions(cv_list, ftell(cv_list), true, false);
            Companies[comp_num].set_positions_list_ptr(cv_list,
            		&(Companies[comp_num].Positions), Companies[comp_num].num_of_positions,
					ftell(cv_list));
            //Companies[comp_num].set_positions_list(cv_list, ftell(cv_list));
        }

        if (strstr(row, "Date: ") || feof(cv_list)) break;
        memset(row,'\0',sizeof(row));
    }
    fseek(cv_list, point_pos, SEEK_SET);

    return 0;
}

int Record::add_new_companies(int num_of_new_comps)
{
	/*
    Companies = (Company*) realloc(Companies, sizeof(Company) * 
    (num_of_companies + num_of_new_comps));

    if(!Companies)
    {
        printf("Memory reallocation for Companies class variable has failed.\n");
        return -1;
    }
    */
	init_companies(&Companies, num_of_companies, num_of_new_comps);
    char row[100];
    char choice;
    int num_of_new_pos;
    uint i;
    for(i = num_of_companies; i < num_of_companies + num_of_new_comps; i++)
    {
       // system("clear");
        printf("\nNumber of companies added to the record: %i.\n",
                num_of_companies + num_of_new_comps);
        printf("Type the name of Company No.%i: ", i+1);
        fgets(row, 100, stdin);
        Companies[i].set_name_ptr(Companies[i].company_name,row);
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
        Companies[i].get_num_of_positions_ptr(NULL ,&(Companies[i].num_of_positions),
        		0 ,false ,true );

        //Companies[i].get_num_of_positions(NULL, 0, false, true);
        Companies[i].add_new_positions_ptr(&(Companies[i].Positions),
        		&(Companies[i].num_of_positions),num_of_new_pos );
        //Companies[i].add_new_positions(num_of_new_pos);
    }

    num_of_companies+= num_of_new_comps;
    return 0;
}

int Record::update_company(int comp_num)
{
    char choice;
    char row[100];
    int num_of_pos = 0;
    int sum_of_pos = Companies[comp_num -1].get_num_of_positions_ptr(NULL,
    		&(Companies[comp_num -1].num_of_positions), 0, false, false);
   // int sum_of_pos = Companies[comp_num -1].get_num_of_positions(NULL, 0,
   //         false, false);
    do
    {
        //system("clear");
        printf("\nCompany No.%i is updated.\n", comp_num);
        printf("Number of positions assigned to the company: %i.\n",
                sum_of_pos + num_of_pos);
        printf("Do you want to add a new job position [Y/N]? ");
        choice = getc(stdin);
        getchar();
        if(choice == 'Y') num_of_pos++;
    } while(choice != 'N' || num_of_pos == 0);

    Companies[comp_num -1].get_num_of_positions_ptr(NULL,
    		&(Companies[comp_num -1].num_of_positions),	0, false, false);

   // Companies[comp_num -1].get_num_of_positions(NULL, 0,
   //     false, false);
    Companies[comp_num -1].add_new_positions_ptr(&(Companies[comp_num -1].Positions),
    		&(Companies[comp_num -1].num_of_positions),num_of_pos);
    //Companies[comp_num -1].add_new_positions(num_of_pos);

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
    uint i;
    for(i = 0; i < num_of_companies; i++)
    {
        printf("\tCompany No.%i: %s\n",i + 1, Companies[i].company_name);
        Companies[i].print_positions_ptr(Companies[i].Positions,
        		Companies[i].num_of_positions);
    }
}

void Record::save_new_companies_in_cv_list(FILE* cv_list)
{
	uint i;
    for(i = 0; i< num_of_companies; i++)
    {
        fprintf(cv_list, "\n\tCompany: %s", Companies[i].company_name);
        Companies[i].save_new_positions_in_cv_list_ptr(cv_list, Companies[i].Positions,
        		Companies[i].num_of_positions);
        //Companies[i].save_new_positions_in_cv_list(cv_list);
    }
}

