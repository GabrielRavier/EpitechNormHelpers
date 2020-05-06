#pragma once
#include <string_view>

namespace diagnostic
{

enum class display_errno
{
	dont,
	doit
};

[[noreturn]] void fatal_error(std::string_view error_description, display_errno should_display_errno);
void error(std::string_view error_description, display_errno should_display_errno);

}
