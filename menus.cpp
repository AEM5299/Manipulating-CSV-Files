#include <vector>
#include <string>
#include <iostream>
#include "menus.h"
#include "ifunctions.h"
#include "csv.h"
#include "types.h"


/*
*	The main menu to control the program....
*	@param		my_csv_file		a csv_file that containt read file data.
*	@param		operations		a vector to use for different operations
*	@param		summary			a vector to save a summary of the operations the user did.
*/
void main_menu(csv_file &my_csv_file)
{
	main_menu_options option;
	do
	{
		std::cout << "What do you want to do with the data?" << std::endl;
		std::cout << "1: Check a column against a range" << std::endl;
		std::cout << "2: Calculate average, min and max for a column" << std::endl;
		std::cout << "3: Evaluate Data (general rating for your data) NOT IMPLEMENTED YET" << std::endl;
		std::cout << "4: Save a copy of your summary to a file" << std::endl;
		std::cout << "5: Quit" << std::endl;
		option = get_option("Enter Your Option: ");

		switch(option)
		{
			case OPTION_CHECK_RANGE:
			{
				int idx = get_col_to_operate_on(my_csv_file);		//choose a header/column
				//does this header/column exist?
				if (idx != -1)
				{
					check_against_range(my_csv_file, idx);
				}
				else std::cout << "There are no such header" << std::endl;
				break;
			}
			case OPTION_CALCULATE_AVG_MIN_MAX:
			{
				int idx = get_col_to_operate_on(my_csv_file);		//choose a header/column
				//does this header/column exist?
				if (idx != -1)
				{
					get_avg_min_max(my_csv_file, idx);
				}
				else std::cout << "There are no such header" << std::endl;
				break;
			}
			case OPTION_EVALUATE_DATA:
			{
				//not actived yet!
				break;
			}
			case OPTION_OUTPUT_SUMMARY:
				output_summary(my_csv_file);
				break;
			case OPTION_EXIT:
				break;
			default:
				std::cout << "Please choose an appropiate choice" << std::endl;
		}

	} while (option != OPTION_EXIT);
}