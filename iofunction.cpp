#include <string>
#include <iostream>
#include "menus.h"

std::string read_text(std::string msg)
{
	//std::cin.ignore(32727, '\n');
	std::cout << msg;
	std::string text;
	std::cin >> text;

	return text;
}

int get_number(std::string msg)
{
	std::cout << msg;
	int number;
	std::cin >> number;											//Reads user input into the variable
	while (std::cin.fail())										//to keep looping if the user enterd a invalid value, in this case, if the user entered anything other than numbers
	{
		//if reached this point, it means that the extraction (reading) falied
		std::cin.clear();										//to refresh cin status, to put it bach to normal mode
		std::cin.ignore(32767, '\n');							//to clear the cin buffer from unwanted inputs
		std::cout << "Please Enter a valid whole number: ";
		std::cin >> number;
	}
	std::cin.ignore(32767, '\n');

	return number;
}

double get_number_double(std::string msg)
{
	std::cout << msg;
	double number;
	std::cin >> number;											//Reads user input into the variable
	while (std::cin.fail())										//to keep looping if the user enterd a invalid value, in this case, if the user entered anything other than numbers
	{
		//if reached this point, it means that the extraction (reading) falied
		std::cin.clear();										//to refresh cin status, to put it bach to normal mode
		std::cin.ignore(32767, '\n');							//to clear the cin buffer from unwanted inputs
		std::cout << "Please Enter a valid whole number: ";
		std::cin >> number;
	}
	std::cin.ignore(32767, '\n');

	return number;
}

main_menu_options get_option(std::string msg)
{
	int number = get_number(msg);

	return static_cast<main_menu_options>(number - 1);
}

