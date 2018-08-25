#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <stdexcept>
#include "csv.h"
#include "splashkit.h"
#include "types.h"
#include "ifunctions.h"

/*
*	A function that reads a .csv file into a 2D vector of strings.
*	@param		file_name		the file name to read
*	@param		my_file_info	the variable that file will be saved to.
*	@return		bool			true if the file is opened succesfully. false otherwise.
*/
bool read_csv(const char file_name[], csv_file &my_file_info)
{
	std::ifstream my_file;
	my_file.open(file_name);		//open the file

	if(my_file.is_open())			//is file opened?
	{
		std::string line;
		std::string line2;
		//reading process could be improved a lot.
		while (std::getline(my_file, line, '\n'))		//save the entire line into the variable
		{
			std::stringstream temp (line);				//save the line read in a stringstream, so we could perform opertations on it.
			std::vector<std::string> dummy;				//dummy vector, saving data temporarily
			while (temp.good())							//is temp stringstream good? (didn't reach the end/no errors occured)
			{
				std::string substr;						//substring
				std::getline(temp, substr, ',');		//save data to the next comma in the substring
				dummy.push_back(substr);				//push the substring in dummy vector
			}
			my_file_info.vector.push_back(dummy);		//push entire dummy vector into our file's vector
		}
		my_file.close();								//close file
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

/*
*	Converts a vector of strings, to a vector of doubles
*	@param		my_csv_file			the file's data
*	@param		idx					the index if the column we want to convert
*	@return		bool				true if file reading succeeded, false otherwise
*/
bool convert_csv_data_double(csv_file &my_csv_file, int idx)
{
	static int last_index = -1;			//to keep it's value when the function exit
	
	if (last_index == idx)
		return true;					//the vector already have the data, we don't need to convert it again
	
	my_csv_file.operations_vector.clear();
	for(int i = my_csv_file.has_headers? 1 : 0; i < my_csv_file.vector.size(); i++)
	{
		try 
		{
			my_csv_file.operations_vector.push_back(std::stod(my_csv_file.vector[i][idx]));
		} catch (const std::invalid_argument& ia)
		{
			std::cout << "This header containts non-numbers data at " << get_index_alignment(my_csv_file, i) << std::endl;
			return false;
		} catch (const std::out_of_range& ia)
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
void check_against_range(csv_file &my_csv_file, int idx)
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

		//The array of data which will be passed to the update function.
		double arr[] = {min, max};
		update_summary(my_csv_file, idx, arr, less_than, more_than);

	}
	
}

/*
*	Calculate the average of a column, along with the minimu and the maximum. And shows them to user.
*	@param		my_csv_file		The file's data
*	@param		idx				The index of the 
*/
void get_avg_min_max(csv_file &my_csv_file, int idx)
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

		//The array of data which will be passed to the update function.
		double arr[] = {min, sum / my_csv_file.operations_vector.size(), max, (double)min_idx, (double)max_idx};
		update_summary(my_csv_file, idx, arr);

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
void output_summary(csv_file &my_file)
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
int get_index_alignment(csv_file &my_file, int idx)
{
	if (idx == -1) return 0;		//out of range

	if(my_file.has_headers)			//does the file have headers?
		return idx + 2;

	return idx + 1;
}