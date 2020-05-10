// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include <string>
#include <string_view>

namespace regex_utils
{

std::string escape_string_for_insertion_in_regex(std::string_view string);

}
