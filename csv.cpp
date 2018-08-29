#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cstring>
#include "csv.h"
#include "splashkit.h"
#include "types.h"
#include "ifunctions.h"

/*
*	A function that reads a .csv file into a 2D vector of strings.
*	@param		file_name			the file name to read
*	@param		my_file_info		the variable that file will be saved to.
*	@param		skip_first_line		whether to skip the first line of the file or not (default no)
*	@return		bool				true if the file is opened succesfully. false otherwise.
*/
bool read_csv_file (const char file_name[], string_2d_vector &save_to, bool skip_first_line)
{
	std::ifstream my_file;
	my_file.open(file_name);		//open the file

	if(my_file.is_open())			//is file opened?
	{
		std::string line;
		if (skip_first_line)
			std::getline(my_file, line, '\n');
		//reading process could be improved a lot.
		while (std::getline(my_file, line, '\n'))		//save the entire line into the variable
		{
			std::stringstream temp (line);				//save the line read in a stringstream, so we could perform opertations on it.
			std::vector<std::string> dummy;				//dummy vector, saving data temporarily
			std::string substr;						//substring
			while (std::getline(temp, substr, ','))					//is temp stringstream good? (didn't reach the end/no errors occured)
			{
				dummy.push_back(substr);				//push the substring in dummy vector
			}
			save_to.push_back(dummy);		//push entire dummy vector into our file's vector
		}
		my_file.close();								//close file
		return true;
	}
	else 
	{
		return false;
	}
}

/*
*	A function to read a the main csv file's data
*	@param		file_name		the file name to read
*	@param		my_file_info	the variable that file will be saved to.
*	@return		bool			true if the file is opened succesfully. false otherwise.
*/
bool user_main_csv(const char file_name[], csv_file &my_file_info)
{
	strcpy(my_file_info.file_name, file_name);			//save file name
	if (read_csv_file(file_name, my_file_info.vector))		// did the reading process succeed?
	{
		my_file_info.has_headers = read_boolean("Does your file have headers? (y/n) ");		//do the file have headers? will be used to determine behaviour later
		std::cout << "Your file has been read successfully" << std::endl;
		return true;
	}
	else
	{
		std::cout << "Unable to open file" << std::endl;
		return false;
	}
}

/*
*	a function to print all the header (row 0) in the csx vector.
*	@param		csv_file		the vector to read from
*	@param		seperator		the character printed after each value
*/
void print_headers(const string_2d_vector &csv_file, char seperator)
{
	//to iterate over the entire vector
	for(int i = 0; i < csv_file[0].size(); i++)
	{
		std::cout << csv_file[0][i];
		if (i != csv_file[0].size() - 1) std::cout << seperator; 
	}
}

/*
*	returns the index of a header, by searching for the header value (-1 if not found)
*	@param		headers		the headers vector
*	@param		header		the header we are searching for
*	@return		int			header index
*/
int get_index_by_header(const std::vector<std::string> &headers, const std::string &header)
{
	//added to accomodate user defined operations
	if (trim(to_lowercase(header)) == "nil")
		return -1;
	
	bool duplicate = false;
	int idx = -1;

	//to iterate over the entire vector
	for (int i = 0; i < headers.size(); i++)
	{
		if(trim(to_lowercase(header)) == trim(to_lowercase(headers[i])))	//is it a match?
		{
			if (idx != -1)		//was the idx value changed? (-1 is default)
				duplicate = true;
			idx = i;
		}
	}
	if (duplicate)		//did we find a dublicate during the search
		std::cout << "There are multiple headers with this name. The latest will be used" << std::endl;

	return idx;
}

/*
*	Converts a vector of strings, to a vector of doubles
*	@param		my_csv_file			the file's data
*	@param		idx					the index if the column we want to convert
*	@return		bool				true if file reading succeeded, false otherwise
*/
bool convert_csv_data_double(csv_file &my_csv_file, int idx)
{
	static int last_index = -1;			//to keep it's value when the function exits
	
	if (last_index == idx && !my_csv_file.operations_vector.empty())		//are we trying to convert the same values again?
		return true;					//the vector already have the data, we don't need to convert it again
	
	my_csv_file.operations_vector.clear();			//clear the vector
	//to iterate over the entire vector
	for(int i = my_csv_file.has_headers? 1 : 0; i < my_csv_file.vector.size(); i++)
	{
		try
		{
			my_csv_file.operations_vector.push_back(std::stod(my_csv_file.vector[i][idx]));
		} catch (const std::invalid_argument& ia)		//is an invalid_arguemnt exception get thrown?
		{
			std::cout << "This header containts non-numbers data at " << get_index_alignment(my_csv_file, i) << std::endl;
			return false;
		} catch (const std::out_of_range& ia)		////is an out_of_range exception get thrown?
		{
			std::cout << "This column contain data out of range at " << get_index_alignment(my_csv_file, i) << std::endl;
			return false; 
		}
	}
	last_index = idx;
	return true;
}

/*
*	Checks a column agaisnt a range. Outputs out of range values indices to user.
*	@param		my_csv_file		The file's data
*	@param		idx				The index of the 
*/
void check_against_range(csv_file &my_csv_file, int idx, bool to_update_summary)
{
	//Did the conversion to doubles succeed?
	if (convert_csv_data_double(my_csv_file, idx))
	{
		//is the vector empty?
		if (my_csv_file.operations_vector.size() == 0)
		{
			std::cout << "There are no data!" << std::endl;
			return;
		}

		double min = get_number<double>("What is the minimum? ");		//Ask the user for the wanted minimum value
		double max = get_number<double>("What is the maximum? ");		//Ask the user for the wanted maximum value
		std::vector<int> less_than;										//A vector to save less than minimum values' indices
		std::vector<int> more_than;										//A vector to save more than maximum values' indices

		//To iterate over the entire vector of data
		for(int i = 0; i < my_csv_file.operations_vector.size(); i++)
		{
			if (my_csv_file.operations_vector[i] < min)
				less_than.push_back(i);						//if less than minimum, push index to the vector
			if (my_csv_file.operations_vector[i] > max)
				more_than.push_back(i);						//if more than maximum, push index to the vector
		}

		bool results_to_screen = false;
		if (less_than.size() != 0 || more_than.size() != 0)			//is atleast one of them not empty?
			results_to_screen = read_boolean("Do you want to print the results to the screen? (either way the results will be saved to the summary) (y/n): ");
		
		if (results_to_screen)
		{
			//To iterate over the entire vector of indices
			for(int i = 0; i < less_than.size(); i++)
			{
				if (i == 0)
					std::cout << "The following is the index(s) of the data less than the minimum:-" << std::endl;

				std::cout << get_index_alignment(my_csv_file, less_than[i]);
				if (i == less_than.size() - 1) std::cout << "\n";			//is it the last element?
				else std::cout << ", ";
			}

			//To iterate over the entire vector of indices
			for(int i = 0; i < more_than.size(); i++)
			{
				if (i == 0)
					std::cout << "The following is the index(s) of the data larger than the maximum:-" << std::endl;
				std::cout << get_index_alignment(my_csv_file, more_than[i]);
				if (i == more_than.size() - 1) std::cout << "\n";
				else std::cout << ", ";			//is it the last element?
			}
		}

		if (to_update_summary)
		{
			//The array of data which will be passed to the update summary function.
			double arr[] = {min, max};
			update_summary(my_csv_file, idx, arr, less_than, more_than);
		}

	}
	
}

/*
*	Calculate the average of a column, along with the minimu and the maximum. And shows them to user.
*	@param		my_csv_file		The file's data
*	@param		idx				The index of the 
*/
void get_avg_min_max(csv_file &my_csv_file, int idx, bool to_update_summary)
{
	//Did the conversion to double succeed?
	if (convert_csv_data_double(my_csv_file, idx))
	{
		//is the vector empty?
		if (my_csv_file.operations_vector.size() == 0)
		{
			std::cout << "There are no data!" << std::endl;
			return;
		}

		double min = my_csv_file.operations_vector[0];
		double max = my_csv_file.operations_vector[0];
		double sum = 0;
		int min_idx = 0;
		int max_idx = 0;

		//To iterate over the entire vector of data
		for (int i = 0; i < my_csv_file.operations_vector.size(); i++)
		{
			sum += my_csv_file.operations_vector[i];
			if (my_csv_file.operations_vector[i] > max)
			{
				max = my_csv_file.operations_vector[i];
				max_idx = i;
			}
			if (my_csv_file.operations_vector[i] < min)
			{
				min = my_csv_file.operations_vector[i];
				min_idx = i;
			}
		}

		//Print to user the min, max and avg, all set to 4 decimals places
		std::cout << std::setprecision(4) << "The minimum is: " << min << ". at Index #" << get_index_alignment(my_csv_file, min_idx) << std::endl;
		std::cout << std::setprecision(4) << "The maximum is: " << max << ". at Index #" << get_index_alignment(my_csv_file, max_idx) << std::endl;
		std::cout << std::setprecision(4) << "The average is: " << sum / my_csv_file.operations_vector.size() << std::endl;

		if (to_update_summary)
		{
			//The array of data which will be passed to the update summary function.
			double arr[] = {min, sum / my_csv_file.operations_vector.size(), max, (double)min_idx, (double)max_idx};
			update_summary(my_csv_file, idx, arr);
		}

	}
	
}

/*
*	A procedure to update the summary vector's data. It's an overloaded function, this version updates the
*	min,max and avg parts of the summary.
*	@param		my_csv_file		The opened file's data
*	@param		index			The index of the column the operation performed on
*	@param		data			a pointer to an array have the min, avg, max, min index and max index values, at that same order
*/
void update_summary(csv_file &my_csv_file, int index, const double* data)
{
	bool exist = false;
	int idx = -1;
	//does our file have header? if yes, assign the header to our variable, if no, assign the index plus one.
	std::string header = my_csv_file.has_headers? my_csv_file.vector[0][index] : std::to_string(index + 1);

	//to iterate over the entire vector of summaries
	for (int i = 0; i < my_csv_file.summary.size(); i++)
	{
		//Did we find a matching header?
		if (header == my_csv_file.summary[i].header)
		{
			exist = true;
			idx = i;
		}
	}

	//the data array follows the following order:
	//data = {min, avg, max, min_idx, max_idx}
	if (exist)
	{
		//it seems that we found a match earlier, so we will ust update the data
		my_csv_file.summary[idx].min_value = data[0];
		my_csv_file.summary[idx].avg = data[1];
		my_csv_file.summary[idx].max_value = data[2];
		my_csv_file.summary[idx].min_idx = data[3];
		my_csv_file.summary[idx].max_idx = data[4];
	}
	else
	{
		//looks like this is a new header, so let's create a new element
		csv_summary temp;
		temp.header = header;
		temp.min_value = data[0];
		temp.avg = data[1];
		temp.max_value = data[2];
		temp.min_idx = (int)data[3];
		temp.max_idx = (int)data[4];
		my_csv_file.summary.push_back(temp);
	}
}

/*
*	A procedure to update the summary vector's data. It's an overloaded function, this version updates the
*	less/more than parts of the summary.
*	@param		my_csv_file		The opened file's data
*	@param		index			The index of the column the operation performed on
*	@param		data			a pointer to an array have the min, max values, at that same order
*	@param		less_than		The vector containing all indices of less_than data
*	@param		more_than		The vector containing all indices of more_than data
*/
void update_summary(csv_file &my_csv_file, int index, const double* data, const std::vector<int> &less_than, const std::vector<int> &more_than)
{
	bool exist = false;
	int idx = -1;
	//does our file have header? if yes, assign the header to our variable, if no, assign the index plus one.
	std::string header = my_csv_file.has_headers? my_csv_file.vector[0][index] : std::to_string(index + 1);

	//to iterate over the entire vector of summaries
	for (int i = 0; i < my_csv_file.summary.size(); i++)
	{
		if (header == my_csv_file.summary[i].header)
		{
			//Did we find a matching header?
			exist = true;
			idx = i;
		}
	}

	//the data array follows the following order:
	//data = {min, max}
	if (exist)
	{
		//it seems that we found a match earlier, so we will ust update the data
		my_csv_file.summary[idx].min = data[0];
		my_csv_file.summary[idx].max = data[1];
		my_csv_file.summary[idx].less_than = less_than;
		my_csv_file.summary[idx].more_than = more_than;
	}
	else
	{
		//looks like this is a new header, so let's create a new element
		csv_summary temp;
		temp.header = header;
		temp.max = data[1];
		temp.min = data[0];
		temp.less_than = less_than;
		temp.more_than = more_than;
		my_csv_file.summary.push_back(temp);
	}
}

/*
*	A procedure to output a summary of the operations performed, to a txt file.
*	indeices printed are user indeices (for more information read get_index_alignment documentation)
*	@param		my_file		the opened file's data
*/
void output_summary(const csv_file &my_file)
{
	std::ofstream output_file;
	output_file.open("Output.txt", std::ofstream::trunc);		//Open the file, and discard any contents
	if (output_file.is_open())					//did we successfully opened the file?
	{
		output_file << "=== Summary === \n\n";
		//to iterate over our entire vector of summaries
		for (int i = 0; i < my_file.summary.size(); i++)
		{
			//Output a summary of the operations performed on the file
			output_file << "== " << my_file.summary[i].header << " ==\n";
			//Output min/max and avg value with its corresponding index
			output_file << "Minimum Value: " << my_file.summary[i].min_value << "(idx #" << get_index_alignment(my_file, my_file.summary[i].min_idx) << ")\n";
			output_file << "Average Value: " << my_file.summary[i].avg << "\n";
			output_file << "Maximum Value: " << my_file.summary[i].max_value << "(idx #" << get_index_alignment(my_file, my_file.summary[i].max_idx) << ")\n";

			//does the vector have data?
			if (my_file.summary[i].less_than.size() != 0)
			{
				output_file << "Values less than " << my_file.summary[i].min << " index(s) are:-\n";
				//to iterate over the entire vector
				for (int j = 0; j < my_file.summary[i].less_than.size(); j++)
				{
					output_file << get_index_alignment(my_file, my_file.summary[i].less_than[j]);
					//is it the last element? if yes, print a new line, otherwise just a comma
					j != (my_file.summary[i].less_than.size() - 1)? output_file << ", " : output_file << "\n";
				}
			}
			
			//does the vector have data?
			if (my_file.summary[i].more_than.size() != 0)
			{
				output_file << "Values more than " << my_file.summary[i].max << " index(s) are:-\n";
				//to iterate over the entire vector
				for (int j = 0; j < my_file.summary[i].more_than.size(); j++)
				{
					output_file << get_index_alignment(my_file, my_file.summary[i].more_than[j]);
					//is it the last element? if yes, print a new line, otherwise just a comma
					j != (my_file.summary[i].more_than.size() - 1)? output_file << ", " : output_file << "\n";
				}
			}
			output_file << "\n\n\n";
		}
		output_file.close();
		std::cout << "DONE!" << std::endl;
	}
	//Couldn't open the file for some reason
	else std::cout << "Falied! Please make sure Output.txt is not used by any other processor" << std::endl;
}

/*
*	A function to facilitate choosing a column to operate on, wheather the file have headers or not.
*	@param		my_file		The file's data
*	@return		int			A column index, -1 if no valid column choosed
*/
int get_col_to_operate_on (const csv_file &my_file)
{
	int idx = -1;		//default value
	if (my_file.has_headers)		//does the file have headers?
		idx = get_index_by_header(my_file.vector[0], read_text("Which header do you want to operate on? "));
	else		//file doesn't have headers
		idx = get_number<int>("Which column do you want to choose?(Enter its number starting from 1) ", 1, my_file.vector[0].size()) - 1;

	return idx;
}

/*
*	A function to convert C-index to a user friendly index, that the user can easily corespond to
*	 in their file. If users file have headers (which get ignored in the operations_vector), the
*	 user index is 2 plus C-index, otherwise 1 plus C-index.
*	@param		my_file		The file's data
*	@param		idx			C-index we want to convert.
*	@return		int			The User-index
*/
int get_index_alignment(const csv_file &my_file, int idx)
{
	if (idx == -1) return 0;		//out of range

	if(my_file.has_headers)			//does the file have headers?
		return idx + 2;

	return idx + 1;
}

/*
*	This function reads the user defined operatiaons file into a
*	 2d string vector.
*	@param		my_file		main csv file info
*/
void user_defined_operations(csv_file &my_file)
{
	string_2d_vector operations;		//a vector to save the data to
	read_csv_file("UserOperations.csv", operations, true);		//read the file into operations vector

	parse_user_operations(operations, my_file);			//translate the data
}

/*
*	a function to translate the data read by user_defined_operations
*	 to values we can use then by the program.
*	@param		ops			2d string vector which the data was saved into
*	@param		my_file		main csv file info
*/
void parse_user_operations(const string_2d_vector &ops, csv_file &my_file)
{
	my_file.user_operations.clear();		//clear the vector from any data
	for (int i = 0; i < ops.size(); i++)
	{
		custom_user_operations dummy;		//just a dummy vector to temporarily save the data into
		dummy.name = ops[i][0];													//idx 0 is operation's name
		dummy.row_1 = get_index_by_header(my_file.vector[0], ops[i][1]);		//idx 1 is row_1
		dummy.operation_1 = get_math_operation(ops[i][2]);						//idx 2 is math operator 1
		dummy.row_2 = get_index_by_header(my_file.vector[0], ops[i][3]);		//idx 3 is row_2
		dummy.operation_2 = get_math_operation(ops[i][4]);						//idx 4 is math operator 2
		dummy.number = std::stod(ops[i][5]);									//idx 5 is multiplier
		user_range_to_array(dummy.excellent_range, ops[i][6]);					//idx 6 is excellent range
		user_range_to_array(dummy.accepted_range1, ops[i][7]);					//idx 7 is accepted range 1
		user_range_to_array(dummy.accepted_range2, ops[i][8]);					//idx 8 is accepted range 2
		if (dummy.row_1 != -1)
			my_file.user_operations.push_back(dummy);				//push to user_operations vector
		else
			std::cout << "Operation #" << i + 1 << "wasn't saved. Row 1 must be valid" << std::endl;
	}

}

/*
*	function to convert the mathematical operator, to one of
*	 math_operations options. +,-,* and / only is supported.
*	@param		operation			The operator string
*	@return		math_operations		The mathematical operation
*/
math_operations get_math_operation (const std::string &math_sign)
{
	if (math_sign == "+")
		return MATH_ADDITION;
	else if (math_sign == "-")
		return MATH_SUBSTRACTION;
	else if (math_sign == "*")
		return MATH_MULTIPLICATION;
	else if (math_sign == "/")
		return MATH_DIVISION;
	else return MATH_MAX_NUM;
}

#ifdef _DEBUG_
/*
*	a function used during debug to print the operations in sequence of ints
*/
void print_user_operation (const csv_file &my_file)
{
	for(int i = 0; i < my_file.user_operations.size(); i++)
	{
		std::cout << my_file.user_operations[i].name << ", ";
		std::cout << my_file.user_operations[i].row_1 << ", ";
		std::cout << my_file.user_operations[i].operation_1 << ", ";
		std::cout << my_file.user_operations[i].row_2 << ", ";
		std::cout << my_file.user_operations[i].operation_2 << ", ";
		std::cout << my_file.user_operations[i].number << ", ";
		std::cout << my_file.user_operations[i].excellent_range[0] << " - ";
		std::cout << my_file.user_operations[i].excellent_range[1] << ", ";
		std::cout << my_file.user_operations[i].accepted_range1[0] << " - ";
		std::cout << my_file.user_operations[i].accepted_range1[1] << ", ";
		std::cout << my_file.user_operations[i].accepted_range2[0] << " - ";
		std::cout << my_file.user_operations[i].accepted_range2[1] << std::endl;
	}
}
#endif

/*
*	performs the custom operation that has the passed index
*	@param		my_file		main csv file info
*	@param		idx			Index of desired custom operation
*/
void do_custom_operation(csv_file &my_file, int idx)
{
	if (idx == -1 || idx > my_file.user_operations.size())		//is the index valid?
		return;
	
	const int& index_1 = my_file.user_operations[idx].row_1;			//just an alias, for shroter function calls
	const int& index_2 = my_file.user_operations[idx].row_2;			//just an alias, for shroter function calls
	const custom_user_operations& rule = my_file.user_operations[idx];	//just an alias, for shroter function calls
	std::vector<range_option> result_vec;			//a vector to save the results into
	
	//to iterate over our entire vector
	for (int i = 1; i < my_file.vector.size(); i++)
	{
		//get the result
		double result = perform_math_op(rule, std::stod(my_file.vector[i][index_1]), index_2 == -1? 0 : std::stod(my_file.vector[i][index_2]), my_file.user_operations[idx].number);
		result_vec.push_back(check_range_custom(result, rule));			//push result to vector
	}

	my_file.user_operations[idx].results = result_vec;					//save results vector to main csv info
	std::cout << "Operation Done! results are saved summary, use the export option to inspect them" << std::endl;
}

/*
*	converts a string of 2 numbers seperated by a /, into an array of 2 elements
*	@param		array			a pointer to the array we want to save the data into
*	@apram		range_String	the string containing the 2 numbers
*/
void user_range_to_array(double array[], const std::string &range_string)
{
	if (trim(to_lowercase(range_string)) == "nil")		//does the string contains nil?
		return;			//leave the default data (0,0)
	
	std::string::size_type pos = range_string.find('/');		//find the position of '/' in the string
	if (pos != range_string.npos)
	{
		array[0] = stod(trim(range_string.substr(0, pos)));			//index 0 is from the start of string till the '/' position
		array[1] = stod(trim(range_string.substr(pos + 1)));		//index 1 is from the '/' position +1 till end
	}
}

/*
*	performs a mathematical operation, and returns the result.
*	@param		rule	The custom operation we are performing
*	@param		val1	Value of row_1
*	@param		val2	value of row_2
*	@param		val3	value of multiplier
*	@return		double	the result
*/
double perform_math_op(const custom_user_operations &rule, double val1, double val2, double val3)
{
	double result = val1;

	if (rule.row_2 != -1)		//is the row 2 index valid?
	{
		if (rule.operation_1 == MATH_ADDITION)
			result += val2;
		else if (rule.operation_1 == MATH_SUBSTRACTION)
			result -= val2;
		else if (rule.operation_1 == MATH_MULTIPLICATION)
			result *= val2;
		else if (rule.operation_1 == MATH_DIVISION)
			result /= val2;
	}
	
	if (rule.operation_2 == MATH_ADDITION)
		result += val3;
	else if (rule.operation_2 == MATH_SUBSTRACTION)
		result -= val3;
	else if (rule.operation_2 == MATH_MULTIPLICATION)
		result *= val3;
	else if (rule.operation_2 == MATH_DIVISION)
		result /= val3;

	return result;
}

/*
*	a function to check a value against the range specified in a custom operation
*	@param		value			The value we to check
*	@param		rule			The custom operation
*	@return		range_option	The range it belongs to
*/
range_option check_range_custom(double value, const custom_user_operations &rule)
{
	//is the value inside the excellent range inclusive? (range limits must be different)
	if (rule.excellent_range[0] != rule.excellent_range[1] && value >= rule.excellent_range[0] && value <= rule.excellent_range[1])
		return RANGE_EXCELLENT;
	//is the value inside one of the accepted ranges inclusive? (range limits must be different)
	if ((rule.accepted_range1[0] != rule.accepted_range1[1] && value >= rule.accepted_range1[0] && value <= rule.accepted_range1[1])
		|| (rule.accepted_range2[0] != rule.accepted_range2[1] && value >= rule.accepted_range2[0] && value <= rule.accepted_range2[1]))
		return RANGE_ACCEPTED;
	//neither inside the accepted range nor the excellent range
	return RANGE_BAD;
	
}

/*
*	exports the results of our custom operations to a text file.
*	@param		my_file		main csv file info
*/
void export_custom_ops_summary(const csv_file &my_file)
{
	std::ofstream output_file;				//file stream
	output_file.open("Custom_Output.txt", std::ofstream::trunc);		//Open the file, and discard any contents
	if (output_file.is_open())					//did we successfully opened the file?
	{
		output_file << "==== User Operations Summary ====" << std::endl;
		custom_ops_export_options option = custom_ops_get_option();			//get user choice to what to export
		//to iterate over our entire vector
		for (int i = 0; i < my_file.user_operations.size(); i++)
		{
			if (my_file.user_operations[i].results.size() == 0)		//if the results vector empty, skip the iteration
				continue;
			output_file << "== " << my_file.user_operations[i].name << " ==" << std::endl;		//name of operation

			{
				output_file << "Out-of-range values indices: ";
				bool comma = false;
				//to iterate over our entire vector
				for(int j = 0; j < my_file.user_operations[i].results.size(); j++)
				{
					if(my_file.user_operations[i].results[j] != RANGE_BAD)			//is value not classified as range bad?
						continue;													//skip the iteration
					if (comma) output_file << ", ";
					output_file << get_index_alignment(my_file, j);				//print the value's index
					comma = true;
				}
			}

			if (option == EXPORT_ACCEPTED || option == EXPORT_ALL)		//if chosen option was including accepted range
			{
				output_file << "\n";
				output_file << "Accepted values indices: ";
				bool comma = false;
				//to iterate over our entire vector
				for(int j = 0; j < my_file.user_operations[i].results.size(); j++)
				{
					if(my_file.user_operations[i].results[j] != RANGE_ACCEPTED)			//is value not classified as range accepted?
						continue;														//skip the iteration
					if (comma) output_file << ", ";
					output_file << get_index_alignment(my_file, j);				//print the value's index
					comma = true;
				}
			}

			if (option == EXPORT_ALL)		//if chosen option was including excellent range
			{
				output_file << "\n";
				output_file << "Excellent values indices: ";
				bool comma = false;
				//to iterate over our entire vector
				for(int j = 0; j < my_file.user_operations[i].results.size(); j++)
				{
					if(my_file.user_operations[i].results[j] != RANGE_EXCELLENT)			//is value not classified as range excellent?
						continue;															//skip the iteration
					if (comma) output_file << ", ";
					output_file << get_index_alignment(my_file, j);				//print the value's index
					comma = true;
				}
			}
			output_file << "\n======================" << std::endl;
		}
		output_file.close();			//close the file
		std::cout << "Summary exported to Custom_Output.txt" << std::endl;
	}
	else
		std::cout << "Error while opening the file. Please make sure Custom_Output.txt is not used by another process" << std::endl;
}

/*
*	lets the user to choose of the the exports option
*	@return		custom_ops_export_options	the export option choosen
*/
custom_ops_export_options custom_ops_get_option()
{
	std::cout << "What do you want to export?" << std::endl;
	std::cout << "1: Only Out of Range values" << std::endl;
	std::cout << "2: Accepted and Out of Range values" << std::endl;
	std::cout << "3: All" << std::endl;

	int option = get_number<int>("Your Choice: ", 1, 3);		//get number as int, between 1 and 3 inclusive
	return static_cast<custom_ops_export_options>(option - 1);	//cast the chosen number to a custom_ops_export_options value
}

/*
*	reloads the main csv file, to capture any changes made to the file
*	@param		my_file		main csv file info
*/
void reload_file(csv_file &my_file)
{
	//clearing the vectors
	my_file.operations_vector.clear();
	my_file.summary.clear();
	my_file.user_operations.clear();
	my_file.vector.clear();

	while (!user_main_csv(my_file.file_name, my_file))		//reading failed
	{
		std::cout << "Press Enter to try again...";
		std::cin.ignore(32767, '\n');
	}
	std::cout << "Reloading Done" << std::endl;
}