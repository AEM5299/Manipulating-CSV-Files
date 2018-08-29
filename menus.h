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
	OPTION_OUTPUT_SUMMARY,
	OPTION_CUSTOM,
	OPTION_RELOAD_FILE,
	OPTION_EXIT,
	OPTION_MAX_NUM
};

/*
*	The main menu to control the program....
*	@param		my_csv_file		a csv_file that contain file's data.
*/
void main_menu(csv_file &my_csv_file);

/*
*	user defined operations sub-menu
*	@param		my_file		main csv file info
*/
void menu_custom_operation(csv_file &my_file);

#endif