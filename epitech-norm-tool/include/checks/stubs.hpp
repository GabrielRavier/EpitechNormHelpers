// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include <string_view> // for std::string_view

namespace checks::stubs
{

void done_in_clang_format(std::string_view check_name);
void done_in_clang_tidy(std::string_view check_name);
void done_in_compiler_and_clang_tidy(std::string_view check_name);
void unimplementable(std::string_view check_name);

} // namespace checks::stubs
