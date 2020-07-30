#include "checks/stubs.hpp"
#include "diagnostic.hpp" // for diagnostic::error
#include <fmt/core.h>	  // for fmt::format
#include <string_view>	  // for std::string_view

#define STR_NOT_IMPLEMENTED_PLS_LOOK_AT "this check is not implemented in the tool, you may want to"
#define STR_CLANG_FORMAT_USE "the provided .clang-format file"
#define STR_CLANG_TIDY_LOOK_AT "clang-tidy.txt"
#define STR_GNU_COMPILER_LOOK_AT "gnu-warnings.txt"
#define STR_FOR_NOTES_HOW_DO_THIS_CHECK_WITH "for notes on how to do this check with"
#define STR_GNU_COMPILER_WITH "your compiler (if you use a GNU-like compiler like GCC or Clang)"
#define STR_CLANG_TIDY_WITH "clang-tidy"

void checks::stubs::done_in_clang_format(std::string_view check_name)
{
	diagnostic::error(fmt::format("{}: " STR_NOT_IMPLEMENTED_PLS_LOOK_AT " use " STR_CLANG_FORMAT_USE, check_name));
}

void checks::stubs::done_in_clang_tidy(std::string_view check_name)
{
	diagnostic::error(fmt::format("{}: " STR_NOT_IMPLEMENTED_PLS_LOOK_AT " look at " STR_CLANG_TIDY_LOOK_AT " " STR_FOR_NOTES_HOW_DO_THIS_CHECK_WITH " " STR_CLANG_TIDY_WITH, check_name));
}

void checks::stubs::done_in_compiler(std::string_view check_name)
{
	diagnostic::error(fmt::format("{}: " STR_NOT_IMPLEMENTED_PLS_LOOK_AT " look at " STR_GNU_COMPILER_LOOK_AT " " STR_FOR_NOTES_HOW_DO_THIS_CHECK_WITH " " STR_GNU_COMPILER_WITH, check_name));
}

void checks::stubs::done_in_compiler_and_clang_tidy(std::string_view check_name)
{
	diagnostic::error(fmt::format("{}: " STR_NOT_IMPLEMENTED_PLS_LOOK_AT " look at " STR_GNU_COMPILER_LOOK_AT " or " STR_CLANG_TIDY_LOOK_AT " " STR_FOR_NOTES_HOW_DO_THIS_CHECK_WITH " " STR_GNU_COMPILER_WITH " or " STR_CLANG_TIDY_WITH, check_name));
}

void checks::stubs::unimplementable(std::string_view check_name)
{
	diagnostic::error(fmt::format("{} unimplemented and unimplementable", check_name));
}
