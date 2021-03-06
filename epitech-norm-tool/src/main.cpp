// SPDX-License-Identifier: GPL-3.0-or-later
#include "diagnostic.hpp" // for diagnostic::fatal_error
#include "options.hpp"	  // for options_parser::parse_options
#include "program.hpp"	  // for program
#include <cstdlib>		  // for std::exit
#include <cxxopts.hpp>	  // for cxxopts::OptionParseException
#include <exception>	  // for std::exception
#include <fmt/core.h>	  // for fmt::format

[[noreturn]] int main(int argc, char *argv[])
{
	try
	{
		program(options_parser::parse_options(argc, argv));
		std::exit(EXIT_SUCCESS);
	}
	catch (const cxxopts::OptionParseException &option_parse_exception)
	{
		diagnostic::fatal_error(fmt::format("failed to parse options: {}", option_parse_exception.what()));
	}
	catch (const std::exception &exception)
	{
		diagnostic::fatal_error(fmt::format("exception thrown: {}", exception.what()));
	}
	catch (...)
	{
		diagnostic::fatal_error("exception thrown: Something not inherited from std::exception");
	}
}
