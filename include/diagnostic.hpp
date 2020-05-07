// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include <string_view>

namespace diagnostic
{

[[noreturn]] void fatal_error(std::string_view error_description, bool should_display_errno);
void error(std::string_view error_description, bool should_display_errno);
void warn(std::string_view warning_description, bool should_display_errno);

}
