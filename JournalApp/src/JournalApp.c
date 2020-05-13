#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"
#include "ctype.h"

#include "record.h"

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
    unsigned int rc, num;
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
    init_records(&Records, 0, sum_of_records);
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
            } while(num < 1 || num > sum_of_records);
            
            print_record_data(Records, num);
            break;

        case '2':
            print_all_recorded_data(Records, sum_of_records);
            break;

        case '3':
            unsigned int i;
            for (i = 0; i < sum_of_records; i++)
            {
                Records[i].clean_record_data_ptr(&(Records[i].Companies),
                        &(Records[i].num_of_companies), &(Records[i].day),
                        &(Records[i].month), &(Records[i].year));
            }

            free(Records);
            Records = NULL;

            add_new_record(filename);
            sum_of_records++;
            init_records(&Records, 0, sum_of_records);
            parse_cv_list(filename, Records, sum_of_records);
            break;

        case '4':
            do{
                printf("\nGive the chosen record's index from 1 - %i:",sum_of_records);
                fgets(chosen_rec_num, sizeof(chosen_rec_num), stdin);
                num = atoi(chosen_rec_num);
                memset(chosen_rec_num, '\0', sizeof(chosen_rec_num));
            } while(num < 1 || num > sum_of_records);
            
            update_record(filename, &Records, num, sum_of_records);
            break;

        case '5':
            clean_record_table(Records, sum_of_records);
            printf("\nCV Journal Application is turning off.\n");
            break;

        default:
            break;
        }
    }while(choice != '5');

    return 0;
}

void print_record_data(Record* Records, int rec_num)
{
    Records[rec_num-1].print_date_ptr(Records[rec_num-1].day, Records[rec_num-1].month,
            Records[rec_num-1].year, rec_num);
    Records[rec_num-1].print_companies_ptr(Records[rec_num-1].Companies,
            Records[rec_num-1].num_of_companies);
}

void print_all_recorded_data(Record* Records, int num_of_records)
{
    for (int i = 0; i < num_of_records; i++)
    {
        Records[i].print_date_ptr(Records[i].day, Records[i].month,
                    Records[i].year, i+1);
        Records[i].print_companies_ptr(Records[i].Companies,
                    Records[i].num_of_companies);
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

    Record* new_record;
    init_records(&new_record,0,1);
    new_record->set_date_ptr(&(new_record->day),&(new_record->month),&(new_record->year), date);
    new_record->get_num_of_companies_ptr(NULL, &(new_record->num_of_companies), 0, false, true);
    new_record->add_new_companies_ptr(&(new_record->Companies), &(new_record->num_of_companies),
            num_of_comps);

    FILE* cv_list;
    cv_list = fopen(filename, "a");
    fprintf(cv_list, "\n");
    fprintf(cv_list, "Date: %02i.%02i.%i", new_record->day
        , new_record->month,
        new_record->year);

    new_record->save_new_companies_in_cv_list_ptr(cv_list, &(new_record->Companies),
            new_record->num_of_companies);
    new_record->clean_record_data_ptr(&(new_record->Companies),&(new_record->num_of_companies),
            &(new_record->day), &(new_record->month), &(new_record->year));

    new_record->get_num_of_companies_ptr= NULL;
    new_record->set_list_of_companies_ptr = NULL;
    new_record->add_new_companies_ptr = NULL;
    new_record->update_company_ptr = NULL;
    new_record->print_date_ptr = NULL;
    new_record->set_date_ptr = NULL;
    new_record->print_companies_ptr = NULL;
    new_record->save_new_companies_in_cv_list_ptr = NULL;
    new_record->clean_record_data_ptr = NULL;

    free(new_record);
    new_record = NULL;

    fclose(cv_list);
    return 0;
}

int update_record(const char* filename, Record** Records, int rec_index, int sum_of_records)
{
    int i = 0;
    int point_pos = 0;
    int num_of_comps = 0;
    int sum_of_comps = 0;
    int num;
    char row[100];
    char choice_yes_or_no;
    char choice_update_option;
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

    num_of_comps = 0;

    do
    {
        //system("clear");
        printf("\nRecord No.%i is updated.\n", rec_index);
        printf("Do you want to:\n");
        printf("1) Add new companies to the record?\n");
        printf("2) Update existing company in the record?\n");
        printf("Make your choice: ");
        choice_update_option = getc(stdin);
        getchar();
        switch (choice_update_option)
        {
        case '1':
            sum_of_comps = (*Records)[rec_index-1].get_num_of_companies_ptr(NULL,
                    &((*Records)[rec_index-1].num_of_companies), 0,false, false);
            num_of_comps = 0;
            do
            {
                //system("clear");
                printf("\nNumber of companies assigned to the updated record: %i \n", sum_of_comps + num_of_comps);
                printf("Do you want to add a new company [Y/N]: ");
                choice_yes_or_no = getc(stdin);
                getchar();
                if(choice_yes_or_no == 'Y')num_of_comps++;
                else if(choice_yes_or_no == 'N' && sum_of_comps + num_of_comps == 0)return 1;
            } while(choice_yes_or_no != 'N');

            (*Records)[rec_index-1].add_new_companies_ptr(&((*Records)[rec_index-1].Companies),
                    &((*Records)[rec_index-1].num_of_companies), num_of_comps);
            break;

        case '2':
            sum_of_comps = (*Records)[rec_index-1].get_num_of_companies_ptr(NULL,
                    &((*Records)[rec_index-1].num_of_companies), 0, false, false);

            do{
                printf("\nGive the chosen company's index from 1 - %i:",
                        sum_of_comps);
                fgets(chosen_comp_num, sizeof(chosen_comp_num), stdin);
                num = atoi(chosen_comp_num);
            }while(num < 1 || num > sum_of_comps);

            (*Records)[rec_index-1].update_company_ptr(&((*Records)[rec_index-1].Companies), num);
            break;

        default:
            break;
        }
    }while(choice_update_option != '1' && choice_update_option != '2');

    cv_list = fopen(filename, "r+");
    fseek(cv_list,point_pos, SEEK_SET);
    for(int i = rec_index -1; i < sum_of_records; i++)
    {
        if(i != rec_index-1)
        {
            fprintf(cv_list, "\n");
            fprintf(cv_list, "Date: %02i.%02i.%i", (*Records)[i].day
                , (*Records)[i].month,
                (*Records)[i].year);
        }
        (*Records)[i].save_new_companies_in_cv_list_ptr(cv_list, &((*Records)[i].Companies),
                (*Records)[i].num_of_companies);
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
            cv_records[rec_num].set_date_ptr(&(cv_records[rec_num].day),
                    &(cv_records[rec_num].month), &(cv_records[rec_num].year), date);

            N = cv_records[rec_num].get_num_of_companies_ptr(cv_list,
                    &(cv_records[rec_num].num_of_companies), ftell(cv_list) , true, false);

            cv_records[rec_num].set_list_of_companies_ptr(cv_list, &(cv_records[rec_num].Companies),
                    &(cv_records[rec_num].num_of_companies), ftell(cv_list));
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
        Records[i].clean_record_data_ptr(&(Records[i].Companies), &(Records[i].num_of_companies),
                &(Records[i].day), &(Records[i].month), &(Records[i].year));
    }

    free(Records);
    return 0;
}
