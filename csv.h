#ifndef _CSV_H_
#define _CSV_H_
#include <vector>
#include <string>

struct csv_summary
{
	std::string header;
	double min = 0;
	std::vector<int> less_than;
	double max = 0;
	std::vector<int> more_than;
	double avg = 0;
	double min_value = 0;
	double max_value = 0;
	int min_idx = 0;
	int max_idx = 0;
};

enum update_summay_type
{
	TYPE_RANGE,
	TYPE_AVG_MIN_MAX,
	MAX
};

bool read_csv(const char file_name[], std::vector< std::vector<std::string> > &saving_string);

void print_headers(const std::vector<std::vector<std::string>> &csv_file, const char seperator[]);

int get_index_by_header(const std::vector<std::string> &headers, const std::string &header);

void convert_csv_data_double(const std::vector<std::vector<std::string>> &csv_file, bool ignore_row_zero, int idx, std::vector<double> &result);

void check_against_range(const std::vector<double> &data, double min, double max, const std::string &header, std::vector<csv_summary> &summary);

void get_avg_min_max(const std::vector<double> &data, const std::string &header, std::vector<csv_summary> &summary);

void update_summary(std::vector<csv_summary> &summary, const std::string &header, double min, double max, double avg, int min_idx, int max_idx);

void update_summary(std::vector<csv_summary> &summary, const std::string &header, const std::vector<int> &less_than, double min, const std::vector<int> &more_than, double max);

void output_summary(std::vector<csv_summary> &summary);
//void evaluate_data(std::vector<double> &data, int arr[];)

#endif