#ifndef JAOBJECTS_H
#define JAOBJECTS_H
#include <cstdint>
#include <cstdio>

class Position
{
private:
	char position_name[100];
public:
	char* get_position();
	void set_position_name(const char* name);
	void print_position_name(int pos_name);
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
	char* get_name();
	int calculate_num_of_positions(FILE* cv_list, int point_pos);
	void set_positions_list(FILE* cv_list, int point_pos);
	void print_positions();
	void clean_company_data();
};

class Record
{
public:
	int calculate_num_of_companies(FILE* cv_list, int point_pos);
	int set_list_of_companies(FILE* cv_list, int point_pos);
	void set_date(int* date);
	void print_date(int record_num);
	void print_companies();
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
