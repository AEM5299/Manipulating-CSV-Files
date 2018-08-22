#ifndef _MENUS_H_
#define _MENUS_H_
#include <vector>
#include <string>
#include "csv.h"
#include "types.h"

/*
*	A list of the main menu options
*/
enum main_menu_options
{
	OPTION_CHECK_RANGE,
	OPTION_CALCULATE_AVG_MIN_MAX,
	OPTION_EVALUATE_DATA,
	OPTION_OUTPUT_SUMMARY,
	OPTION_EXIT,
	OPTION_MAX_NUM
};

/*
*	The main menu to control the program....
*	@param		my_csv_file		a csv_file that containt read file data.
*	@param		operations		a vector to use for different operations
*	@param		summary			a vector to save a summary of the operations the user did.
*/
void main_menu(csv_file &my_csv_file);

#endif