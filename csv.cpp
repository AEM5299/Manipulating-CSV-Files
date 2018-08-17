#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include "csv.h"
#include "splashkit.h"

bool read_csv(const char file_name[], std::vector< std::vector<std::string> > &saving_string)
{
	std::ifstream my_file; 
	my_file.open(file_name);

	if(my_file.is_open())
	{
		std::string line;
		std::string line2;
		while (std::getline(my_file, line, '\n'))
		{
			std::stringstream temp (line);
			std::vector<std::string> dummy;
			while (temp.good())
			{
				std::string substr;
				std::getline(temp, substr, ',');
				dummy.push_back(substr);
			}
			saving_string.push_back(dummy);
		}
		my_file.close();
		return true;
	}
	else 
	{
		std::cout << "Unable to open file" << std::endl;
		return false;
	}

}

void print_headers(const std::vector<std::vector<std::string>> &csv_file, const char seperator[])
{
	for(int i = 0; i < csv_file[0].size(); i++)
	{
		std::cout << csv_file[0][i];
		if (i != csv_file[0].size() - 1) std::cout << seperator; 
	}
}

int get_index_by_header(const std::vector<std::string> &headers, const std::string &header)
{
	bool duplicate = false;
	int idx = -1;

	for (int i = 0; i < headers.size(); i++)
	{
		if(to_lowercase(header) == to_lowercase(headers[i]))
		{
			if (idx != -1)
				duplicate = true;
			idx = i;
		}
	}
	if (duplicate) std::cout << "There are multiple headers with this name. The latest will be used" << std::endl;

	return idx;
}

void convert_csv_data_double(const std::vector<std::vector<std::string>> &csv_file, bool ignore_row_zero, int idx, std::vector<double> &result)
{
	result.clear();
	for(int i = ignore_row_zero? 1 : 0; i < csv_file.size(); i++)
	{
		result.push_back(std::stod(csv_file[i][idx]));
	}
}

void check_against_range(const std::vector<double> &data, double min, double max, const std::string &header, std::vector<csv_summary> &summary)
{
	if (data.size() == 0)
		return;
	
	std::vector<int> less_than;
	std::vector<int> more_than;

	for(int i = 0; i < data.size(); i++)
	{
		if (data[i] < min)
			less_than.push_back(i+1);
		if (data[i] > max)
			more_than.push_back(i+1);
	}

	if (less_than.size() != 0)
	{
		std::cout << "The following is the indexes of the data less than the minimum:-" << std::endl;
		for(int i = 0; i < less_than.size(); i++)
		{
			std::cout << less_than[i];
			if (i == less_than.size() - 1) std::cout << "\n";
			else std::cout << ", ";
		}
	}

	if (more_than.size() != 0)
	{
		std::cout << "The following is the indexes of the data larger than the maximum:-" << std::endl;
		for(int i = 0; i < more_than.size(); i++)
		{
			std::cout << more_than[i];
			if (i == more_than.size() - 1) std::cout << "\n";
			else std::cout << ", ";
		}
	}

	update_summary(summary, header, less_than, min, more_than, max);
	
}

void get_avg_min_max(const std::vector<double> &data, const std::string &header, std::vector<csv_summary> &summary)
{
	if (data.size() == 0)
	{
		//error
		return;
	}
	double min = data[0];
	double max = data[0];
	double sum = 0;
	int min_idx = 0;
	int max_idx = 0;

	for (int i = 0; i < data.size(); i++)
	{
		sum += data[i];
		if (data[i] > max)
		{
			max = data[i];
			max_idx = i+1;
		}
		if (data[i] < min)
		{
			min = data[i];
			min_idx = i+1;
		}
	}

	std::cout << std::setprecision(4) << "The minimum is: " << min << ". at Index #" << min_idx << std::endl;
	std::cout << std::setprecision(4) << "The maximum is: " << max << ". at Index #" << max_idx << std::endl;
	std::cout << std::setprecision(4) << "The average is: " << sum / data.size() << std::endl;

	update_summary(summary, header, min, max, sum / data.size(), min_idx, max_idx);
	
}

void update_summary(std::vector<csv_summary> &summary, const std::string &header, double min, double max, double avg, int min_idx, int max_idx)
{
	bool exist = false;
	int idx = -1;
	for (int i = 0; i < summary.size(); i++)
	{
		if (header == summary[i].header)
		{
			exist = true;
			idx = i;
		}
	}

	if (exist)
	{
		summary[idx].avg = avg;
		summary[idx].min_value = min;
		summary[idx].max_value = max;
		summary[idx].min_idx = min_idx;
		summary[idx].max_idx = max_idx;
	}
	else
	{
		csv_summary temp;
		temp.header = header;
		temp.avg = avg;
		temp.min_value = min;
		temp.max_value = max;
		temp.min_idx = min_idx;
		temp.max_idx = max_idx;
		summary.push_back(temp);
	}
}

void update_summary(std::vector<csv_summary> &summary, const std::string &header, const std::vector<int> &less_than, double min, const std::vector<int> &more_than, double max)
{
	bool exist = false;
	int idx = -1;
	for (int i = 0; i < summary.size(); i++)
	{
		if (header == summary[i].header)
		{
			exist = true;
			idx = i;
		}
	}

	if (exist)
	{
		summary[idx].min = min;
		summary[idx].less_than = less_than;
		summary[idx].more_than = more_than;
		summary[idx].max = max;
	}
	else
	{
		csv_summary temp;
		temp.header = header;
		temp.less_than = less_than;
		temp.more_than = more_than;
		temp.max = max;
		temp.min = min;
		summary.push_back(temp);
	}
}

void output_summary(std::vector<csv_summary> &summary)
{
	std::ofstream output_file;
	output_file.open("Output.txt", std::ofstream::trunc);
	if (output_file.is_open())
	{
		output_file << "=== Summary === \n\n";
		for (int i = 0; i < summary.size(); i++)
		{
			std::cout << "inside the for" << std::endl;
			output_file << "== " << summary[i].header << " ==\n";
			output_file << "Minimum Value: " << summary[i].min_value << "\n";
			output_file << "Average Value: " << summary[i].avg << "\n";
			output_file << "Maximum Value: " << summary[i].max_value << "\n";
			std::cout << "before first if" << std::endl;
			if (summary[i].less_than.size() != 0)
			{
				output_file << "Value indexes less than " << summary[i].min << ":-\n";
				for (int j = 0; j < summary[i].less_than.size(); j++)
				{
					output_file << summary[i].less_than[j];
					j != (summary[i].less_than.size() - 1)? output_file << ", " : output_file << "\n";
				}
			}
			std::cout << "before 2nd if" << std::endl;
			if (summary[i].more_than.size() != 0)
			{
				output_file << "Value indexes more than " << summary[i].max << ":-\n";
				for (int j = 0; j < summary[i].more_than.size(); j++)
				{
					output_file << summary[i].more_than[j];
					j != (summary[i].more_than.size() - 1)? output_file << ", " : output_file << "\n";
				}
			}
			output_file << "\n\n\n";
		}
		output_file.close();
	}
	else std::cout << "Falied! Please make sure Output.txt is not opened or used by any other processor" << std::endl;
}