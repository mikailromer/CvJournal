#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cctype>

#include "jaobjects.h"

void print_record_data(Record* Records, int rec_num);
void print_all_recorded_data(Record* Records, int num_of_records);
int add_new_record(const char* filename);
int update_record(const char* filename, Record** Records, int rec_index, int sum_of_records);
int parse_cv_list(const char* filename, Record *cv_records, int number_of_records);
int num_of_records(const char* filename);
int initialize_cv_list_file(const char* filename);
int clean_record_table(Record* Records, int sum_of_records);


int main()
{

    Record* Records = NULL;
    uint32_t sum_of_records;
    const char* filename = "src/cv_list.txt";
    FILE* cv_list = fopen(filename,"r");
    char choice;
    char chosen_rec_num[4];
    int rc, num;
    if(!cv_list)
    {
        printf("The cv_list.txt file doesnt't exist.\n");
        printf("It will be created in the src directory.\n");
        rc = initialize_cv_list_file(filename);
        if(rc) return 1;
    }
    else if(cv_list && num_of_records(filename) == 0)
    {
        rc = initialize_cv_list_file(filename);
        if(rc) return 1;
    }
    else fclose(cv_list);

    sum_of_records = num_of_records(filename);
    Records = (Record*) realloc(Records, sizeof(Record) * sum_of_records);
    parse_cv_list(filename,Records, sum_of_records);

    do{
        printf("\nThe CV Journal application has been properly initialized.\n");
        printf("Number of records present in CV list: %i\n", sum_of_records);
        printf("You have 5 options to chose: \n");
        printf("1 Print the chosen record's data.\n");
        printf("2 Print all of the records data.\n");
        printf("3 Add the new record to the Cv list.\n");
        printf("4 Update existing record in the Cv list.\n");
        printf("5 Exit the program.\n\n");
        printf("Make your choice: ");
        choice = getc(stdin);
        getchar();

        switch (choice)
        {
        case '1':
            do{
                printf("\nGive the chosen record's index from 1 - %i:",sum_of_records);
                fgets(chosen_rec_num, sizeof(chosen_rec_num), stdin);
                num = atoi(chosen_rec_num);
                memset(chosen_rec_num, '\0', sizeof(chosen_rec_num));
            }while(num < 1 || num > sum_of_records);
            
            print_record_data(Records, num);
            break;

        case '2':
            print_all_recorded_data(Records, sum_of_records);
            break;

        case '3':
            for (uint i = 0; i < sum_of_records; i++)
            {
                Records[i].clean_record_data();
            }

            free(Records);
            Records = NULL;

            add_new_record(filename);
            sum_of_records++;
            Records = (Record*) realloc(Records, sizeof(Record) * sum_of_records);
            parse_cv_list(filename, Records, sum_of_records);
            break;

        case '4':
            do{
                printf("\nGive the chosen record's index from 1 - %i:",sum_of_records);
                fgets(chosen_rec_num, sizeof(chosen_rec_num), stdin);
                num = atoi(chosen_rec_num);
                memset(chosen_rec_num, '\0', sizeof(chosen_rec_num));
            }while(num < 1 || num > sum_of_records);
            
            update_record(filename, &Records, num, sum_of_records);
            break;

        case '5':
            printf("\nCV Journal Application is turning off.\n");
            clean_record_table(Records, sum_of_records);
            break;

        default:
            break;
        }
    }while(choice != '4');

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


    //system("clear");
    printf("\nGive date in the following format - DD/MM/YYYY: ");
    fgets(row, 100, stdin);
    if(row[2] != '/' || row[5] != '/')
    {
        return -1;
    }

    for(int j = 0; j < 10;j++)
    {
        if(!(isdigit(row[j])) && j!= 2 && j!=5){
            return -1;
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
        //system("clear");
        printf("\nNumber of added companies to the new record: %i \n", num_of_comps);
        printf("Do you want to add a new company [Y/N]: ");
        choice = getc(stdin);
        getchar();
        if(choice == 'Y')num_of_comps++;
        else if(choice == 'N' && num_of_comps == 0)return 1;
    } while(choice != 'N');

    Record new_record;
    new_record.set_date(date);
    new_record.get_num_of_companies(NULL, 0, false, true);
    new_record.add_new_companies(num_of_comps);

    FILE* cv_list;
    cv_list = fopen(filename, "a");
    fprintf(cv_list, "\n");
    fprintf(cv_list, "Date: %02i.%02i.%i", new_record.get_day()
        , new_record.get_month(),
        new_record.get_year());

    new_record.save_new_companies_in_cv_list(cv_list);
    new_record.clean_record_data();

    fclose(cv_list);
    return 0;
}

int update_record(const char* filename, Record** Records, int rec_index, int sum_of_records)
{
    int i = 1;
    int point_pos = 0;
    int num_of_comps = 0;
    int num;
    char row[100];
    char choice;
    char chosen_comp_num[4];

    FILE* cv_list;
    cv_list = fopen(filename, "r");

    for(;;)
    {
        fgets(row, sizeof(row), cv_list);
        if (strstr(row, "Date:"))i++;
        
        memset(row,'\0', sizeof(row));

        point_pos = ftell(cv_list);
        if(i == rec_index) break;
        if (feof(cv_list)) break;
    }
    fclose(cv_list);

    num_of_comps = (*Records)[rec_index-1].get_num_of_companies(NULL, 0 ,false, false);

    do
    {
        //system("clear");
        printf("\nRecord No.%i is updated.\n", rec_index);
        printf("Do you want to:\n");
        printf("1) Add new companies to the record?\n");
        printf("2) Update existing company in the record?\n");
        printf("Make your choice: ");
        choice = getc(stdin);
        getchar();
        switch (choice)
        {
        case '1':
            do
            {
                //system("clear");
                printf("\nNumber of companies assigned to the updated record: %i \n", num_of_comps);
                printf("Do you want to add a new company [Y/N]: ");
                choice = getc(stdin);
                getchar();
                if(choice == 'Y')num_of_comps++;
                else if(choice == 'N' && num_of_comps == 0)return 1;
            } while(choice != 'N');

            (*Records)[rec_index-1].add_new_companies(num_of_comps);
            break;

        case '2':
            do{
                printf("\nGive the chosen company's index from 1 - %i:",num_of_comps);
                fgets(chosen_comp_num, sizeof(chosen_comp_num), stdin);
                num = atoi(chosen_comp_num);
            }while(num < 1 || num > sum_of_records);

            (*Records)[rec_index-1].update_company(num);

        default:
            break;
        }

    }while(choice != '1' && choice != '2');

    cv_list = fopen(filename, "r+");
    fseek(cv_list,point_pos, SEEK_SET);
    for(int i = rec_index -1; i < sum_of_records; i++)
    {
        if(i != rec_index-1)
        {
            fprintf(cv_list, "\n");
            fprintf(cv_list, "Date: %02i.%02i.%i", (*Records)[rec_index-1].get_day()
                , (*Records)[rec_index-1].get_month(),
                (*Records)[rec_index-1].get_year());
        }
        (*Records)[rec_index-1].save_new_companies_in_cv_list(cv_list);
    }

    fclose(cv_list);
    return 0;
}

int parse_cv_list(const char* filename, Record *cv_records, int number_of_records)
{
    FILE* cv_list =fopen(filename, "r");
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
            N = cv_records[rec_num].get_num_of_companies(cv_list, ftell(cv_list) , true, false);
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

int initialize_cv_list_file(const char* filename)
{
    FILE* cv_list = fopen(filename, "w");
    char choice;
    cv_list = fopen(filename,"w");
    if(!cv_list)
    {
        printf("File cv_list.txt hasn't been created.");
        return 1;
    }
    fprintf(cv_list, "!!!CV List!!!\n");
    fclose(cv_list);
    printf("There is no records in the cv_list file.\n");
    printf("You have 2 options to chose: \n");
    printf("1 Add the first record to the CV list.\n");
    printf("2 Exit the program.\n");
    printf("Chose your option: ");
    do{
        choice = getc(stdin);
        getchar();
    }while (choice != '1' && choice != '2');

    if(choice == '1')
    {
        printf("Option No.1 has been chosen.\n");
        printf("The new record will be added to the Cv list.\n");
        add_new_record(filename);
        return 0;
    }
    else
    {
        printf("Option No.2 has been chosen.\n");
        printf("Program stops its work.\n");
        return 1;
    }
}

int clean_record_table(Record* Records, int sum_of_records )
{
    for (int i = 0; i < sum_of_records; i++)
    {
        Records[i].clean_record_data();
    }

    free(Records);

    return 0;
}
