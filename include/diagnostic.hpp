// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include <string_view>

namespace diagnostic
{

[[noreturn]] void fatal_error(std::string_view error_description);
void error(std::string_view error_description);
void warn(std::string_view warning_description);

} // namespace diagnostic
