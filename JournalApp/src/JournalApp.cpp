#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>

#include "jaobjects.h"

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

			N = cv_records[rec_num].calculate_num_of_companies(cv_list, ftell(cv_list));

			cv_records[rec_num].set_list_of_companies(cv_list, ftell(cv_list));
		}

		if (feof(cv_list)) break;
	}
	fclose(cv_list);

	return 0;
}

void print_recorded_data(Record* Records, int num_of_records)
{
	for (int i = 0; i < num_of_records; i++)
	{
		Records[i].print_date(i+1);
	}
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
		if (feof(cv_list)) break;
	}
	fclose(cv_list);
	return rec_num;
}
int main()
{
	
	Record* Records;
	const char* filename = "src/cv_list.txt";
	uint32_t N = num_of_records(filename);
	if (N) 
	{ 
		Records = (Record*) malloc(sizeof(Record) * N);
		parse_cv_list(filename, Records, N);
		print_recorded_data(Records, N);
		for (int i = 0; i < N; i++)
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
