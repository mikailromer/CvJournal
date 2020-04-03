#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cctype>

#include "jaobjects.h"

void print_record_data(Record* Records, int rec_num);
void print_all_recorded_data(Record* Records, int num_of_records);
int add_new_record(const char* filename);
int parse_cv_list(const char* filename, Record *cv_records, int number_of_records);
int num_of_records(const char* filename);



int main()
{

    Record* Records = NULL;
    const char* filename = "src/cv_list.txt";
    uint32_t N = num_of_records(filename);

    if (N)
    {
        Records = (Record*) realloc(Records, sizeof(Record) * N);
        parse_cv_list(filename, Records, N);
        add_new_record(filename);
        //print_record_data(Records, 21);
        //print_all_recorded_data(Records, N);
        N = num_of_records(filename);
        Records = (Record*) realloc(Records, sizeof(Record) * N);
        parse_cv_list(filename, Records, N);
        for (uint32_t i = 0; i < N; i++)
        {
            Records[i].clean_record_data();
        }

        free(Records);
    }
    else
    {
        return -1;
    }

    return 0;
}

void print_record_data(Record* Records, int rec_num)
{
    Records[rec_num-1].print_date(rec_num);
    Records[rec_num-1].print_companies();
}

void print_all_recorded_data(Record* Records, int num_of_records)
{
    for (int i = 0; i < num_of_records; i++)
    {
        Records[i].print_date(i+1);
        Records[i].print_companies();
    }
}

int add_new_record(const char* filename)
{
    char row[100];
    char* p;
    char choice;
    int num_of_comps = 0;
    int date[3];


    system("clear");
    printf("Give date in the following format - DD/MM/YYYY: ");
    fgets(row, 100, stdin);
    if(row[2] != '/' || row[5] != '/')
    {
        return 1;
    }

    for(int j = 0; j < 10;j++)
    {
        if(!(isdigit(row[j])) && j!= 2 && j!=5){
            return 1;
        }
    }

    int i = 0;
    p = strtok(row, "/");
    while(p != NULL)
    {
        date[i] = atoi(p);
        p = strtok(NULL,"/");
        i++;
    }


    memset(row, '\0', sizeof(row));

    do
    {
        system("clear");

        printf("Number of added companies to the new record: %i \n", num_of_comps);
        printf("Do you want to add a new company [Y/N]: ");
        choice = getc(stdin);
        getchar();
        if(choice == 'Y')num_of_comps++;
        else if(choice == 'N' && num_of_comps == 0)return 1;
    } while(choice != 'N');

    Record new_record;
    //Records = (Record*) realloc(Records, sizeof(Record) * (*number_of_records) );
    new_record.set_date(date);
    new_record.get_num_of_companies(NULL, 0, false);
    new_record.add_new_companies(num_of_comps);

    FILE* cv_list;
    cv_list = fopen(filename, "a");
    fprintf(cv_list, "\n");
    fprintf(cv_list, "Date: %02i.%02i.%i", new_record.get_day()
        , new_record.get_month(),
		new_record.get_year());

    new_record.save_new_companies_in_cv_list(cv_list);

    fclose(cv_list);
    new_record.clean_record_data();

    return 0;
}

int parse_cv_list(const char* filename, Record *cv_records, int number_of_records)
{
    FILE* cv_list =fopen(filename, "r") ;
    if (!cv_list)
    {
        printf("Cannot open file %s.\n", filename);
        return -1;
    }

    char row[100];
    int rec_num = -1;

    int date[3];
    int i;
    int N;
    char *p;
    for (;;)
    {
        fgets(row, sizeof(row), cv_list);
        p= strstr(row, "Date:");
        if (p)
        {
            i = 0;
            p += 6;
            p = strtok(p, " .");
            while (p != NULL)
            {
                date[i] = atoi(p);
                p = strtok(NULL, " .");
                i++;
            }

            rec_num++;
            cv_records[rec_num].set_date(date);
            N = cv_records[rec_num].get_num_of_companies(cv_list, ftell(cv_list) , true);
            cv_records[rec_num].set_list_of_companies(cv_list, ftell(cv_list));
        }

        memset(row,'\0',sizeof(row));
        if (feof(cv_list)) break;
    }
    fclose(cv_list);

    return 0;
}

int num_of_records(const char* filename)
{
    FILE* cv_list =fopen(filename, "r") ;
    if (!cv_list)
    {
        printf("Cannot open file %s.\n", filename);
        return -1;
    }

    char row[100];
    int rec_num = 0;

    for(;;)
    {
        fgets(row, sizeof(row), cv_list);
        if (strstr(row, "Date:"))
        {
            rec_num++;
        }
        memset(row,'\0', sizeof(row));
        if (feof(cv_list)) break;
    }
    fclose(cv_list);
    return rec_num;
}
