// SPDX-License-Identifier: GPL-3.0-or-later
#include "diagnostic.hpp"
#include <cstdlib>	   // for std::exit, EXIT_FAILURE
#include <fmt/core.h>  // for std::format
#include <iostream>	   // for std::cerr
#include <string>	   // for operator<<(std::ostream&, std::string)
#include <string_view> // for std::string_view

static void do_diagnostic_print(std::string_view error_description, std::string_view error_kind)
{
	std::cerr << fmt::format("{}: {}\n", error_kind, error_description);
}

[[noreturn]] void diagnostic::fatal_error(std::string_view error_description)
{
	do_diagnostic_print(error_description, "fatal error");

	std::exit(EXIT_FAILURE);
}

void diagnostic::error(std::string_view error_description)
{
	do_diagnostic_print(error_description, "error");
}

void diagnostic::warn(std::string_view error_description)
{
	do_diagnostic_print(error_description, "warning");
}
