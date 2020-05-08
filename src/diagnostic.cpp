// SPDX-License-Identifier: GPL-3.0-or-later
#include <string_view>
#include <iostream>
#include <stdio.h>
#include <fmt/format.h>
#include "diagnostic.hpp"

static std::string getErrnoString(int error_number)
{
	std::string result;

#if (_POSIX_C_SOURCE >= 200112L) && !_GNU_SOURCE

	size_t result_size = 50;
	result.resize(result_size);

	while (strerror_r(error_number, result.data(), result_size) == -1 && errno == ERANGE)
	{
		result_size *= 2;
		result.resize(result_size);
	}

#else

	char buffer[10000];
	result = strerror_r(error_number, buffer, sizeof(buffer));	// A copy is made here since the string may only live until buffer dies

#endif

	return result;
}

static void do_diagnostic_print(std::string_view error_description, bool should_display_errno, std::string_view error_kind)
{
	std::cerr << fmt::format("{}: {}\n", error_kind, error_description);

	if (should_display_errno)
		std::cerr << fmt::format("corresponding system error: {}\n", getErrnoString(errno));
}

[[noreturn]] void diagnostic::fatal_error(std::string_view error_description, bool should_display_errno)
{
	do_diagnostic_print(error_description, should_display_errno, "fatal error");

	exit(EXIT_FAILURE);
}

void diagnostic::error(std::string_view error_description, bool should_display_errno)
{
	do_diagnostic_print(error_description, should_display_errno, "error");
}

void diagnostic::warn(std::string_view error_description, bool should_display_errno)
{
	do_diagnostic_print(error_description, should_display_errno, "warning");
}