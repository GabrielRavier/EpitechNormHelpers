#include "basename.hpp"
#include <string>
#include <string_view>
#include <algorithm>
#include <libgen.h>
#include <iostream>

std::string basename_wrappers::base_name(const std::string& path)
{
	// basename may modify the input, so we make a copy
	std::string copy = path;
	std::string result = basename(copy.data());

	return result;
}

std::string basename_wrappers::directory_base_name(const std::string& path)
{
	// dirname may modify the input, so we make a copy
	std::string copy = path;
	std::string result = dirname(copy.data());

	return result;
}
