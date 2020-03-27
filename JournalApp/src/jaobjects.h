#ifndef JAOBJECTS_H
#define JAOBJECTS_H
#include <cstdint>
#include <cstdio>

class Position
{
private:
	char position_name[100];
public:
	void get_position();
	void set_position_name(const char* name);
};

class Company
{
private:
	char company_name[100];
	Position* Positions;
	uint32_t num_of_positions;

public:
	Company();
	~Company();
	void set_name(const char *name);
	int calculate_num_of_positions(FILE* cv_list, int point_pos);
	void set_positions_list(FILE* cv_list, int point_pos);
	void allocate_memory_for_positions();
	void get_company();
};

class Record
{
public:
	int calculate_num_of_companies(FILE* cv_list, int point_pos);
	int set_list_of_companies(FILE* cv_list, int point_pos);
	void set_date(int* date);
	void print_date(int record_num);
	void clean_record_data();
	Record();
	~Record();

private:
	uint8_t day;
	uint8_t month;
	uint64_t year;
	Company *Companies;
	int num_of_companies;
};

#endif // !JAOBJECTS
