// SPDX-License-Identifier: GPL-3.0-or-later
#include <cxxopts.hpp>
#include <fmt/format.h>
#include <exception>
#include <iostream>
#include "options.hpp"
#include "program.hpp"

int main(int argc, char *argv[])
{
	try
	{
		program(options_parser::parse_options(argc, argv));
		return EXIT_SUCCESS;
	}
	catch (const cxxopts::OptionParseException& option_parse_exception)
	{
		std::cerr << fmt::format("Failed to parse options: {}\n", option_parse_exception.what());
	}
	catch (const std::exception& exception)
	{
		std::cerr << fmt::format("Exception thrown: {}\n", exception.what());
	}
	catch (...)
	{
		std::cerr << "Exception thrown: Something not inherited from std::exception\n";
	}

	return EXIT_FAILURE;
}
