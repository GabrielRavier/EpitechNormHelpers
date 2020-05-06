#pragma once
#include <string_view>

namespace diagnostic
{

[[noreturn]] void fatal_error(std::string_view error_description, bool display_errno);
void error(std::string_view error_description, bool display_errno);

}
