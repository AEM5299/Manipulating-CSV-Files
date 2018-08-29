#ifndef _CSV_H_
#define _CSV_H_
#include <vector>
#include <string>
#include "types.h"

//Math operations supported
enum math_operations
{
	MATH_ADDITION,
	MATH_SUBSTRACTION,
	MATH_MULTIPLICATION,
	MATH_DIVISION,
	MATH_MAX_NUM
};

//range options
enum range_option
{
	RANGE_EXCELLENT,
	RANGE_ACCEPTED,
	RANGE_BAD,
	RANGE_MAX_NUM
};

//export custom operations options
enum custom_ops_export_options
{
	EXPORT_BAD,
	EXPORT_ACCEPTED,
	EXPORT_ALL,
	EXPORT_MAX_NUM
};

//user operations structure
struct custom_user_operations
{
	std::string name;
	int row_1;
	math_operations operation_1;
	int row_2;
	math_operations operation_2;
	double number;
	double excellent_range [2] = {0.0, 0.0};	//default values
	double accepted_range1 [2] = {0.0, 0.0};
	double accepted_range2 [2] = {0.0, 0.0};
	std::vector<range_option> results;
};

//main summary structure
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

//csv_file structure
struct csv_file
{
	string_2d_vector vector;
	bool has_headers = true;
	std::vector<double> operations_vector;
	std::vector<csv_summary> summary;
	std::vector<custom_user_operations> user_operations;
	char file_name[];
};

/*
*	A function that reads a .csv file into a 2D vector of strings.
*	@param		file_name			the file name to read
*	@param		my_file_info		the variable that file will be saved to.
*	@param		skip_first_line		whether to skip the first line of the file or not (default no)
*	@return		bool				true if the file is opened succesfully. false otherwise.
*/
bool read_csv_file (const char file_name[], string_2d_vector &save_to, bool skip_first_line = false);

/*
*	A function to read a the main csv file's data
*	@param		file_name		the file name to read
*	@param		my_file_info	the variable that file will be saved to.
*	@return		bool			true if the file is opened succesfully. false otherwise.
*/
bool user_main_csv(const char file_name[], csv_file &my_file_info);

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
void check_against_range(csv_file &my_csv_file, int idx, bool to_update_summary = true);

/*
*	Calculate the average of a column, along with the minimu and the maximum. And shows them to user.
*	@param		my_csv_file		The file's data
*	@param		idx				The index of the 
*/
void get_avg_min_max(csv_file &my_csv_file, int idx, bool to_update_summary = true);

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

/*
*	This function reads the user defined operatiaons file into a
*	 2d string vector.
*	@param		my_file		main csv file info
*/
void user_defined_operations(csv_file &my_file);

/*
*	a function to translate the data read by user_defined_operations
*	 to values we can use then by the program.
*	@param		ops			2d string vector which the data was saved into
*	@param		my_file		main csv file info
*/
void parse_user_operations(const string_2d_vector &ops, csv_file &my_file);

/*
*	function to convert the mathematical operator, to one of
*	 math_operations options. +,-,* and / only is supported.
*	@param		operation			The operator string
*	@return		math_operations		The mathematical operation
*/
math_operations get_math_operation (const std::string &math_sign);

#ifdef _DEBUG_
/*
*	a function used during debug to print the operations in sequence of ints
*/
void print_user_operation (const csv_file &my_file);
#endif

/*
*	performs the custom operation that has the passed index
*	@param		my_file		main csv file info
*	@param		idx			Index of desired custom operation
*/
void do_custom_operation(csv_file &my_file, int idx);

/*
*	converts a string of 2 numbers seperated by a /, into an array of 2 elements
*	@param		array			a pointer to the array we want to save the data into
*	@apram		range_String	the string containing the 2 numbers
*/
void user_range_to_array(double array[], const std::string &range_string);

/*
*	performs a mathematical operation, and returns the result.
*	@param		rule	The custom operation we are performing
*	@param		val1	Value of row_1
*	@param		val2	value of row_2
*	@param		val3	value of multiplier
*	@return		double	the result
*/
double perform_math_op(const custom_user_operations &rule, double val1, double val2, double val3);

/*
*	a function to check a value against the range specified in a custom operation
*	@param		value			The value we to check
*	@param		rule			The custom operation
*	@return		range_option	The range it belongs to
*/
range_option check_range_custom(double value, const custom_user_operations &rule);

/*
*	exports the results of our custom operations to a text file.
*	@param		my_file		main csv file info
*/
void export_custom_ops_summary(const csv_file &my_file);

/*
*	lets the user to choose of the the exports option
*	@return		custom_ops_export_options	the export option choosen
*/
custom_ops_export_options custom_ops_get_option();

/*
*	reloads the main csv file, to capture any changes made to the file
*	@param		my_file		main csv file info
*/
void reload_file(csv_file &my_file);

#endif