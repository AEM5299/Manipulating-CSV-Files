#ifndef _MENUS_H_
#define _MENUS_H_
#include <vector>
#include <string>
#include "csv.h"

enum main_menu_options
{
	OPTION_CHECK_RANGE,
	OPTION_CALCULATE_AVG_MIN_MAX,
	OPTION_EVALUATE_DATA,
	OPTION_OUTPUT_SUMMARY,
	OPTION_EXIT,
	OPTION_MAX_NUM
};

void main_menu(std::vector< std::vector<std::string> > &csv_file, std::vector<double> &operations, std::vector<csv_summary> &summary);

#endif