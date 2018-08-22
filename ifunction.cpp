#include <string>
#include <iostream>
#include "menus.h"
#include "ifunctions.h"
#include "splashkit.h"

/*
*	Prompts the user for input and reads the input and then will return
*	as a string.
*	@param		msg			The text that will be displayed to the user
*	@return		string		The whole line of text that user entered
*/
std::string read_text(std::string msg)
{
	std::cout << msg;
	std::string text;
	//std::getline used to fix whitespaces issue
	std::getline(std::cin, text);				//Reads input into the variable

	return text;
}

/*
*	Prompts the user for input and returns it as Type
*	@param		msg			The text that will be displayed to the user
*	@param		min			The minimum value that will be accepted
*	@param		max			The maximum value that would be accepted (if both min and max = 0, the function will accept any number)
*	@return		Type		The number that the user entered
*/
template <class Type>
Type get_number(std::string msg, Type min, Type max)
{
	std::cout << msg;
	Type number;
	std::cin >> number;											//Reads user input into the variable
	while (std::cin.fail())										//to keep looping if the user enterd a invalid value, in this case, if the user entered anything other than numbers
	{
		//if reached this point, it means that the extraction (reading) falied
		std::cin.clear();										//to refresh cin status, to put it bach to normal mode
		std::cin.ignore(32767, '\n');							//to clear the cin buffer from unwanted inputs
		std::cout << "Please enter a valid number: ";
		std::cin >> number;
	}
	std::cin.ignore(32767, '\n');

	if (min != 0 && max != 0 && (number > max || number < min))			//if there is a range passed, and the entry doesn't fit in the range
	{
		std::cout << "You entered an out of range number (" << min << " - " << max << "), Please try again." << std::endl;
		get_number<Type>(msg, min, max);								//Ask to enter again
	}

	return number;
}
template int get_number(std::string msg, int min, int max);
template double get_number(std::string msg, double min, double max);
template float get_number(std::string msg, float min, float max);

/*
*	Prompts the user for input, and convert it to an appropiate 
*	main_men_options value
*	@param		msg					The text that will be displayed to the user
*	@return		main_menu_options	The option the user entered
*/
main_menu_options get_option(std::string msg)
{
	int number = get_number<int>(msg);

	return static_cast<main_menu_options>(number - 1);
}

/*
*	Prompts the user to input either yes or no, and returns as a bool value.
*	If user typed any text that is not accpeted, an appropiate error message
*	will be displayed.
*	@param		msg			The prompt that will be displayed to the user
*	@return		bool		return either true or false, depending on the user's
*							entry.
*/
bool read_boolean(std::string msg)
{
	std::string text = to_lowercase(trim(read_text(msg)));						//to trim any leading or/and trailing whitespaces, and convert all the letters to lowercase

	while(text != "yes" && text != "y" && text != "no" && text != "n")		//to loop until user enter accepted value
	{
		std::cout << "Please write yes (y) or no (n) only!" << std::endl;
		text = to_lowercase(trim(read_text(msg)));
	}

	if (text == "yes" || text == "y")
		return true;
	else if (text == "no" || text == "n")
		return false;

	return false;			//just in case something goes horribly wrong
}

