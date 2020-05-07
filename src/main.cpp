// SPDX-License-Identifier: GPL-3.0-or-later
#include <cxxopts.hpp>
#include <fmt/format.h>
#include <exception>
#include <iostream>
#include "options.hpp"
#include "program.hpp"
#include "diagnostic.hpp"

int main(int argc, char *argv[])
{
	try
	{
		program(options_parser::parse_options(argc, argv));
		return EXIT_SUCCESS;
	}
	catch (const cxxopts::OptionParseException& option_parse_exception)
	{
		diagnostic::fatal_error(fmt::format("failed to parse options: {}", option_parse_exception.what()), false);
	}
	catch (const std::exception& exception)
	{
		diagnostic::fatal_error(fmt::format("exception thrown: {}", exception.what()), false);
	}
	catch (...)
	{
		diagnostic::fatal_error("exception thrown: Something not inherited from std::exception", false);
	}

	// Unreachable
}
