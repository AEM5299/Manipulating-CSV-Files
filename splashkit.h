#ifndef _SPLASH_KIT_H
#define _SPLASH_KIT_H

#include <string>

/*
*	Copied from splashkit source code. Convert all letters in a string to lowercase
*	@param		text		string to convert
*	@return		string		converted string
*/
std::string to_lowercase(const std::string &text);

//trim from beginning. Copied from Splashkit.
std::string ltrim(const std::string &text);

// trim from end. Copied from Splashkit.
std::string rtrim(const std::string &text);

// trim from both ends. Copied from Splashkit.
std::string trim(const std::string &text);

#endif