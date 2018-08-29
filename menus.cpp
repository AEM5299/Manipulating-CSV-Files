#include <vector>
#include <string>
#include <iostream>
#include "menus.h"
#include "ifunctions.h"
#include "csv.h"
#include "types.h"


/*
*	The main menu to control the program....
*	@param		my_csv_file		a csv_file that contain file's data.
*/
void main_menu(csv_file &my_csv_file)
{
	main_menu_options option;
	do
	{
		std::cout << "What do you want to do with the data?" << std::endl;
		std::cout << "1: Check a column against a range" << std::endl;
		std::cout << "2: Calculate average, min and max for a column" << std::endl;
		std::cout << "3: Save a copy of your summary to a file" << std::endl;
		std::cout << "4: User Defined Operations" << std::endl;
		std::cout << "5: Reload file (history will be lost)" << std::endl;
		std::cout << "6: Quit" << std::endl;
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
			case OPTION_CUSTOM:
				menu_custom_operation(my_csv_file);
				break;
			case OPTION_OUTPUT_SUMMARY:
				output_summary(my_csv_file);
				break;
			case OPTION_RELOAD_FILE:
				reload_file(my_csv_file);
				break;
			case OPTION_EXIT:
				break;
			default:
				std::cout << "Please choose an appropiate choice" << std::endl;
		}

	} while (option != OPTION_EXIT);
}

/*
*	user defined operations sub-menu
*	@param		my_file		main csv file info
*/
void menu_custom_operation(csv_file &my_file)
{
	user_defined_operations(my_file);		//read the user defined operations file
	int option;
	do {
		if (my_file.user_operations.size() == 0)
			std::cout << "There are NO operations" << std::endl;
		else
		{
			//to iterate over the entire vector
			for(int i = 0; i < my_file.user_operations.size(); i++)
			{
				std::cout << i + 1 << ": " << my_file.user_operations[i].name << std::endl;		//printout the operations name
			}
		}

		//additional options
		std::cout << my_file.user_operations.size() + 1 << ": Export Summary" << std::endl;
		std::cout << my_file.user_operations.size() + 2 << ": re-load operations file (history will be lost)" << std::endl;
		std::cout << my_file.user_operations.size() + 3 << ": Back (history will be lost)" << std::endl;

		option = get_number<int>("Choose an option: ", 1, my_file.user_operations.size() + 3);		//prompt the user to enter an option (1, size + 3)

		if (option == my_file.user_operations.size() + 1)
			export_custom_ops_summary(my_file);			//export summary
		else if (option == my_file.user_operations.size() + 2)
			user_defined_operations(my_file);			//reload operations file
		else
			do_custom_operation(my_file, option - 1);	//perform the operation
	} while (option != my_file.user_operations.size() + 3);
}