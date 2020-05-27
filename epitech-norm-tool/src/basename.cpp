// SPDX-License-Identifier: GPL-3.0-or-later
#include "basename.hpp"
#include <libgen.h>	// for dirname, basename
#include <string>	// for std::string
#include <string_view>	// for std::string_view

std::string basename_wrappers::base_name(std::string_view path)
{
	// basename may modify the input, so we make a copy
	auto copy = std::string{path};
	std::string result = basename(copy.data());

	return result;
}

std::string basename_wrappers::directory_base_name(std::string_view path)
{
	// dirname may modify the input, so we make a copy
	auto copy = std::string{path};
	std::string result = dirname(copy.data());

	return result;
}
