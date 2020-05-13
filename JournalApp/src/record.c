#include "record.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

static int get_num_of_companies(FILE* cv_list, uint32_t* num_of_comps,
        int point_pos,  bool getFromCvList, bool isRecordNew);
static int set_list_of_companies(FILE *cv_list,Company** Companies,
        uint32_t* num_of_comps, int point_pos);
static int add_new_companies(Company** Companies, uint32_t* num_of_comps,
        int num_of_new_comps);
static int update_company(Company** Companies, int comp_num);
static void set_date(uint8_t* day, uint8_t* month, uint32_t* year, int* date);
static void print_date(uint8_t day, uint8_t month, uint32_t year, int record_num);
static void print_companies(Company* Companies, uint32_t num_of_comps);
static void save_new_companies_in_cv_list(FILE* cv_list,Company** Companies,
        uint32_t num_of_comps);

static void clean_record_data(Company** Companies, uint32_t* num_of_comps,
        uint8_t* day, uint8_t* month,  uint32_t* year);


int init_records(Record** Records, unsigned int num_of_recs,
        unsigned int num_of_new_recs)
{
    unsigned int begin, end, i;
    if(num_of_recs == 0) *Records = NULL;
    *Records = (Record*) realloc(*Records, sizeof(Record)
            * (num_of_recs + num_of_new_recs));
    if(*Records == NULL)
    {
        printf("Memory reallocation for Records variable has failed.\n");
        return -1;
    }

    if(num_of_recs == 0)
    {
        begin = 0;
        end = num_of_new_recs;
    }
    else
    {
        begin = num_of_recs;
        end = num_of_recs + num_of_new_recs;
    }

    for(i = begin; i < end; i++)
    {
        (*Records)[i].get_num_of_companies_ptr= get_num_of_companies;
        (*Records)[i].set_list_of_companies_ptr = set_list_of_companies;
        (*Records)[i].add_new_companies_ptr = add_new_companies;
        (*Records)[i].update_company_ptr = update_company;
        (*Records)[i].print_date_ptr = print_date;
        (*Records)[i].set_date_ptr = set_date;
        (*Records)[i].print_companies_ptr = print_companies;
        (*Records)[i].save_new_companies_in_cv_list_ptr = save_new_companies_in_cv_list;
        (*Records)[i].clean_record_data_ptr = clean_record_data;

        (*Records)[i].Companies = NULL;
        (*Records)[i].day = 0;
        (*Records)[i].month = 0;
        (*Records)[i].year = 0;
        (*Records)[i].num_of_companies = 0;
    }

    return 0;
}

static int get_num_of_companies(FILE* cv_list, uint32_t* num_of_comps,
        int point_pos,  bool getFromCvList, bool isRecordNew)
{
    if(getFromCvList)
    {
        *num_of_comps = 0;
        char row[100];
        for (;;)
        {
            fgets(row, sizeof(row), cv_list);
            if (strstr(row, "Company:"))(*num_of_comps)++;
            if (strstr(row, "Date: ") || feof(cv_list)) break;
            memset(row,'\0', sizeof(row));
        }

        fseek(cv_list, point_pos, SEEK_SET);
    }
    else if(!getFromCvList && isRecordNew) *num_of_comps = 0;

    return *num_of_comps;
}



static int set_list_of_companies(FILE *cv_list,Company** Companies,
        uint32_t* num_of_comps, int point_pos)
{
    char row[100];
    int comp_num = -1;
    char* p;

    init_companies(Companies, 0, *num_of_comps);
    for (;;)
    {
        fgets(row, sizeof(row), cv_list);
        p = strstr(row, "Company: ");
        if (p)
        {
            p += 9;
            comp_num++;
            (*Companies)[comp_num].set_name_ptr((*Companies)
                    [comp_num].company_name, p);

            (*Companies)[comp_num].get_num_of_positions_ptr(cv_list,
                    &((*Companies)[comp_num].num_of_positions),
                    ftell(cv_list), true, false);

            (*Companies)[comp_num].set_positions_list_ptr(cv_list,
                    &((*Companies)[comp_num].Positions),
                    (*Companies)[comp_num].num_of_positions,
                    ftell(cv_list));
        }

        if (strstr(row, "Date: ") || feof(cv_list)) break;
        memset(row,'\0',sizeof(row));
    }
    fseek(cv_list, point_pos, SEEK_SET);

    return 0;
}

static int add_new_companies(Company** Companies, uint32_t* num_of_comps,
        int num_of_new_comps)
{
    init_companies(Companies, *num_of_comps, num_of_new_comps);
    char row[100];
    char choice;
    int num_of_new_pos;
    unsigned int i;
    for(i = *num_of_comps; i < *num_of_comps + num_of_new_comps; i++)
    {
       // system("clear");
        printf("\nNumber of companies added to the record: %i.\n",
                *num_of_comps + num_of_new_comps);
        printf("Type the name of Company No.%i: ", i+1);
        fgets(row, 100, stdin);
        (*Companies)[i].set_name_ptr((*Companies)[i].company_name,row);
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
        (*Companies)[i].get_num_of_positions_ptr(NULL ,
                &((*Companies)[i].num_of_positions),
                0 ,false ,true );

        (*Companies)[i].add_new_positions_ptr(&((*Companies)[i].Positions),
                &((*Companies)[i].num_of_positions),num_of_new_pos );

    }

    *num_of_comps+= num_of_new_comps;
    return 0;
}

static int update_company(Company** Companies, int comp_num)
{
    char choice;
    char row[100];
    int num_of_pos = 0;
    int sum_of_pos = (*Companies)[comp_num -1].get_num_of_positions_ptr(NULL,
            &((*Companies)[comp_num -1].num_of_positions), 0, false, false);

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

    (*Companies)[comp_num -1].get_num_of_positions_ptr(NULL,
            &((*Companies)[comp_num -1].num_of_positions),	0, false, false);

    (*Companies)[comp_num -1].add_new_positions_ptr(&((*Companies)
            [comp_num -1].Positions),&((*Companies)[comp_num -1].num_of_positions),
            num_of_pos);

    return 0;
}


static void set_date(uint8_t* day, uint8_t* month, uint32_t* year, int* date)
{
    *day = date[0];
    *month = date[1];
    *year = date[2];
}


static void print_date(uint8_t day, uint8_t month, uint32_t year, int record_num)
{
    printf("\n");
    printf("Record No.%i\n",record_num);
    printf("Date: %i.%i.%i\n", day, month, year);
}


static void print_companies(Company* Companies, uint32_t num_of_comps)
{
    printf("\n");
    unsigned int i;
    for(i = 0; i < num_of_comps; i++)
    {
        printf("\tCompany No.%i: %s\n",i + 1, Companies[i].company_name);
        Companies[i].print_positions_ptr(Companies[i].Positions,
                Companies[i].num_of_positions);
    }
}

static void save_new_companies_in_cv_list(FILE* cv_list,Company** Companies,
        uint32_t num_of_comps)
{
    unsigned int i;
    for(i = 0; i< num_of_comps; i++)
    {
        fprintf(cv_list, "\n\tCompany: %s", (*Companies)[i].company_name);
        (*Companies)[i].save_new_positions_in_cv_list_ptr(cv_list,
                (*Companies)[i].Positions, (*Companies)[i].num_of_positions);
    }
}

static void clean_record_data(Company** Companies, uint32_t* num_of_comps,
        uint8_t* day, uint8_t* month,  uint32_t* year)
{
    Position* Positions;
    unsigned int i;
    for(i = 0; i < *num_of_comps; i++)
    {
        Positions = (*Companies)[i].Positions;
        (*Companies)[i].clean_company_data_ptr(&Positions,
                &((*Companies)[i].num_of_positions),
                (*Companies)[i].company_name);
    }
    free(*Companies);

    *Companies = NULL;
    *num_of_comps = NULL;
    *day = NULL;
    *month = NULL;
    *year = NULL;
}
