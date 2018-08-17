#ifndef _IOFUNCTION_H_
#define _IOFUNCTION_H_
#include <string>

std::string read_text(std::string msg);

int get_number(std::string msg);

double get_number_double(std::string msg);

main_menu_options get_option(std::string msg);

#endif