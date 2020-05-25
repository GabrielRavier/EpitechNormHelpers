// SPDX-License-Identifier: GPL-3.0-or-later
#include "diagnostic.hpp"
#include <fmt/format.h>
#include <iostream>
#include <stdio.h>
#include <string_view>

static void do_diagnostic_print(std::string_view error_description, std::string_view error_kind)
{
	std::cerr << fmt::format("{}: {}\n", error_kind, error_description);
}

[[noreturn]] void diagnostic::fatal_error(std::string_view error_description)
{
	do_diagnostic_print(error_description, "fatal error");

	exit(EXIT_FAILURE);
}

void diagnostic::error(std::string_view error_description)
{
	do_diagnostic_print(error_description, "error");
}

void diagnostic::warn(std::string_view error_description)
{
	do_diagnostic_print(error_description, "warning");
}
