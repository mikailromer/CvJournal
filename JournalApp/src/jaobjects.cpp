#include "jaobjects.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>



void Record::clean_record_data()
{
	free(Companies);
}

int Record::calculate_num_of_companies(FILE* cv_list, int point_pos)
{
	char row[100];
	int i = 0;
	num_of_companies = 0;
	for (;;)
	{
		fgets(row, sizeof(row), cv_list);
		if (strstr(row, "Company: "))num_of_companies++;
		if (strstr(row, "Date: ") || feof(cv_list)) break;
	}
	fseek(cv_list, point_pos, SEEK_SET);
	
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
		if (strstr(row, "Date: ") || feof(cv_list)) break;
		
		p = strstr(row, "Company: ");
		if (p)
		{
			p += 9;
			comp_num++;
			Companies[comp_num].set_name(p);
			Companies[comp_num].calculate_num_of_positions(cv_list, ftell(cv_list));
			//Companies[comp_num].allocate_memory_for_positions();
			//Companies[comp_num].set_positions_list(cv_list, ftell(cv_list));
		}			
	}
	fseek(cv_list, point_pos, SEEK_SET);

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
	printf("Record No.%i\n",record_num);
	printf("Date: %i.%i.%i\n", day, month, year);
}
int Company::calculate_num_of_positions(FILE* cv_list, int point_pos)
{
	char row[100];
	int i = 0;
	num_of_positions = 0;
	for (;;)
	{
		fgets(row, sizeof(row), cv_list);
		if (strstr(row, "Position: ")) num_of_positions++;
		if (strstr(row, "Date: ") || strstr(row,"Company: ") || feof(cv_list)) break;
	}
	fseek(cv_list, point_pos, SEEK_SET);

	return num_of_positions;
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
		if (strstr(row, "Date: ") || strstr(row, "Company: ") || feof(cv_list)) break;
		p = strstr(row, "Position: ");
		if (p)
		{
			p += 10;
			position_num++;
			Positions[position_num].set_position_name(p);
		}
	}

	fseek(cv_list, point_pos, SEEK_SET);
}



void Company::set_name(const char* name)
{
	strcpy(company_name, name);
	company_name[strlen(company_name) - 1] = '\0';
}

void Company::allocate_memory_for_positions()
{
	Positions = (Position*) malloc(sizeof(Position) * 7);
	if (!(Positions))
	{
		printf("Memory allocation has failed!!!");
		exit(1);
	}
	free(Positions);
}
void Position::set_position_name(const char* name)
{
	strcpy(position_name, name);
	position_name[strlen(position_name) - 1] = '\0';
}