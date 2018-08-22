#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include "menus.h"
#include "csv.h"
#include "ifunctions.h"
#include "types.h"

int main()
{
	std::cout << "If it's your first time to use the program, please read 'readme.txt' to understand how it works" << std::endl;
	csv_file my_file;		//to save the file data in
	std::string file_name = read_text("What is tha name of the file? (must be in the same directory) ");		//file name?

	while (!read_csv(file_name.c_str(), my_file))		//if reading the file failed
	{
		std::cout << "Please make sure that the file is in the same directory, and is not being used by other processor" << std::endl;
		file_name = read_text("What is tha name of the file? ");
	}
	
	std::cout << "Your file has been read successfully" << std::endl;
	main_menu(my_file);

	return 0;
}