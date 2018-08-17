#include <vector>
#include <string>
#include <iostream>
#include "menus.h"
#include "iofunctions.h"
#include "csv.h"

void main_menu(std::vector< std::vector<std::string> > &csv_file, std::vector<double> &operations, std::vector<csv_summary> &summary)
{
	main_menu_options option;
	do
	{
		std::cout << "What do you want to do with the data?" << std::endl;
		std::cout << "1: Check a column against a range" << std::endl;
		std::cout << "2: Calculate average, min and max for a column" << std::endl;
		std::cout << "3: Evaluate Data (general rating for your data)" << std::endl;
		std::cout << "4: Save a copy of your summary to a file" << std::endl;
		std::cout << "5: Quit" << std::endl;
		option = get_option("Enter Your Option: ");

		switch(option)
		{
			case OPTION_CHECK_RANGE:
			{
				int idx = get_index_by_header(csv_file[0], read_text("Which header do you want to operate on? "));
				if (idx != -1)
				{
					convert_csv_data_double(csv_file, true, idx, operations);
					check_against_range(operations, get_number_double("what is the minimum? "), get_number_double("What is the maximum? "), csv_file[0][idx] , summary);
				}
				else std::cout << "There are no such header" << std::endl;
				break;
			}
			case OPTION_CALCULATE_AVG_MIN_MAX:
			{
				int idx = get_index_by_header(csv_file[0], read_text("Which header do you want to operate on? "));
				if (idx != -1)
				{
					convert_csv_data_double(csv_file, true, idx, operations);
					get_avg_min_max(operations, csv_file[0][idx], summary);
				}
				else std::cout << "There are no such header" << std::endl;
				break;
			}
			/*case OPTION_EVALUATE_DATA:
			{
				
				break;
			}*/
			case OPTION_OUTPUT_SUMMARY:
				output_summary(summary);
				break;
			case OPTION_EXIT:
				break;
			default:
				std::cout << "Please choose an appropiate choice" << std::endl;
		}
	} while (option != OPTION_EXIT);
}