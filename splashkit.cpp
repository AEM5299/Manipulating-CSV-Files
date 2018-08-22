#include <string>
#include <algorithm>

/*
*	Copied from splashkit source code. Convert all letters in a string to lowercase
*	@param		text		string to convert
*	@return		string		converted string
*/
std::string to_lowercase(const std::string &text)
{
	std::string data = text;
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	return data;
}

//trim from beginning
std::string ltrim(const std::string &text)
{
	std::string s = text;
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
std::string rtrim(const std::string &text)
{
	std::string s = text;
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both ends
std::string trim(const std::string &text)
{
	return ltrim(rtrim(text));
}