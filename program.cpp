#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include "menus.h"
#include "csv.h"
#include "iofunctions.h"

int main()
{
	std::cout << "If it's your first time to use the program, please read 'readme.txt' to avoid problems";
	std::vector< std::vector<std::string> > csv_file;
	std::string file_name = read_text("What is tha name of the file? (must be in the same directory) ");
	if(read_csv(file_name.c_str(), csv_file) == true)
	{
		std::vector<double> operations_vector;
		std::vector<csv_summary> summary;
		std::cout << "Your file has been read successfully" << std::endl;
		main_menu(csv_file, operations_vector, summary);
	}

	return 0;
}