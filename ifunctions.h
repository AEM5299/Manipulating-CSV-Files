#ifndef _IFUNCTION_H_
#define _IFUNCTION_H_
#include <string>
#include "menus.h"

/*
*	Prompts the user for input and reads the input and then will return
*	as a string.
*	@param		msg			The text that will be displayed to the user
*	@return		string		The whole line of text that user entered
*/
std::string read_text(std::string msg);

/*
*	Prompts the user for input and returns it as Type
*	@param		msg			The text that will be displayed to the user
*	@param		min			The minimum value that will be accepted
*	@param		max			The maximum value that would be accepted (if both min and max = 0, the function will accept any number)
*	@return		Type		The number that the user entered
*/
template <class Type>
Type get_number(std::string msg, Type min = 0, Type max = 0);

/*
*	Prompts the user for input, and convert it to an appropiate 
*	main_men_options value
*	@param		msg					The text that will be displayed to the user
*	@return		main_menu_options	The option the user entered
*/
main_menu_options get_option(std::string msg);

/*
*	Prompts the user to input either yes or no, and returns as a bool value.
*	If user typed any text that is not accpeted, an appropiate error message
*	will be displayed.
*	@param		msg			The prompt that will be displayed to the user
*	@return		bool		return either true or false, depending on the user's
*							entry.
*/
bool read_boolean(std::string msg);

#endif