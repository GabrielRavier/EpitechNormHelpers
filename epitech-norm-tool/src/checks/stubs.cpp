#include "checks/stubs.hpp"
#include "diagnostic.hpp" // for diagnostic::error
#include <fmt/core.h>	  // for fmt::format
#include <string_view>	  // for std::string_view

void checks::stubs::done_in_clang_format(std::string_view check_name)
{
	diagnostic::error(fmt::format("{}: this check is not implemented in the tool, you may want to look at the provided .clang-format file", check_name));
}

void checks::stubs::done_in_clang_tidy(std::string_view check_name)
{
	diagnostic::error(fmt::format("{}: this check is not implemented in the tool, you may want to look at clang-tidy.txt for notes on how to do this check with clang-tidy", check_name));
}

void checks::stubs::done_in_compiler_and_clang_tidy(std::string_view check_name)
{
	diagnostic::error(fmt::format("{}: this check is not implemented in the tool, you may want to look at gnu-warnings.txt or clang-tidy.txt for notes on how to do this check directly with your compiler or clang-tidy", check_name));
}

void checks::stubs::unimplementable(std::string_view check_name)
{
	diagnostic::error(fmt::format("{} unimplemented and unimplementable", check_name));
}
