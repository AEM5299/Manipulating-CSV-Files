#ifndef _CSV_H_
#define _CSV_H_
#include <vector>
#include <string>
#include <stdexcept>
#include "types.h"

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
	int min_idx = -1;
	int max_idx = -1;
};

struct csv_file
{
	string_2d_vector vector;
	bool has_headers = true;
	std::vector<double> operations_vector;
	std::vector<csv_summary> summary;
};

/*
*	A function that reads a .csv file into a 2D vector of strings.
*	@param		file_name		the file name to read
*	@param		my_file_info	the variable that file will be saved to.
*	@return		bool			true if the file is opened succesfully. false otherwise.
*/
bool read_csv(const char file_name[], csv_file &my_file_info);

/*
*	a function to print all the header (row 0) in the csx vector.
*	@param		csv_file		the vector to read from
*	@param		seperator		the character printed after each value
*/
void print_headers(const string_2d_vector &csv_file, char seperator);

/*
*	returns the index of a header, by searching for the header value (-1 if not found)
*	@param		headers		the headers vector
*	@param		header		the header we are searching for
*	@return		int			header index
*/
int get_index_by_header(const std::vector<std::string> &headers, const std::string &header);

/*
*	Converts a vector of strings, to a vector of doubles
*	@param		my_csv_file			the file's data
*	@param		idx					the index if the column we want to convert
*	@return		bool				true if file reading succeeded, false otherwise
*/
bool convert_csv_data_double(csv_file &my_csv_file, int idx);

/*
*	Checks a column agaisnt a range. Outputs out of range values indices to user.
*	@param		my_csv_file		The file's data
*	@param		idx				The index of the 
*/
void check_against_range(csv_file &my_csv_file, int idx);

/*
*	Calculate the average of a column, along with the minimu and the maximum. And shows them to user.
*	@param		my_csv_file		The file's data
*	@param		idx				The index of the 
*/
void get_avg_min_max(csv_file &my_csv_file, int idx);

/*
*	A procedure to update the summary vector's data. It's an overloaded function, this version updates the
*	min,max and avg parts of the summary.
*	@param		my_csv_file		The opened file's data
*	@param		index			The index of the column the operation performed on
*	@param		data			a pointer to an array have the min, avg, max, min index and max index values, at that same order
*/
void update_summary(csv_file &my_csv_file, int index, const double* data);

/*
*	A procedure to update the summary vector's data. It's an overloaded function, this version updates the
*	less/more than parts of the summary.
*	@param		my_csv_file		The opened file's data
*	@param		index			The index of the column the operation performed on
*	@param		data			a pointer to an array have the min, max values, at that same order
*	@param		less_than		The vector containing all indices of less_than data
*	@param		more_than		The vector containing all indices of more_than data
*/
void update_summary(csv_file &my_csv_file, int index, const double* data, const std::vector<int> &less_than, const std::vector<int> &more_than);

/*
*	A procedure to output a summary of the operations performed, to a txt file.
*	indeices printed are user indeices (for more information read get_index_alignment documentation)
*	@param		my_file		the opened file's data
*/
void output_summary(const csv_file &my_file);

/*
*	A function to facilitate choosing a column to operate on, wheather the file have headers or not.
*	@param		my_file		The file's data
*	@return		int			A column index, -1 if no valid column choosed
*/
int get_col_to_operate_on (const csv_file &my_file);

/*
*	A function to convert C-index to a user friendly index, that the user can easily corespond to
*	 in their file. If users file have headers (which get ignored in the operations_vector), the
*	 user index is 2 plus C-index, otherwise 1 plus C-index.
*	@param		my_file		The file's data
*	@param		idx			C-index we want to convert.
*	@return		int			The User-index
*/
int get_index_alignment(const csv_file &my_file, int idx);

#endif