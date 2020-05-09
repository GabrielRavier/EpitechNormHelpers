// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include <string>

namespace regex_utils
{

std::string escape_string_for_insertion_in_regex(const std::string& string);

}
