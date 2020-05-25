// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include "checks/checks.hpp"
#include <boost/regex.hpp>
#include <string>
#include <string_view>

namespace regex_utils
{

std::string escape_string_for_insertion_in_regex(std::string_view string);
bool simple_regex_match(std::string_view string, const boost::regex &regex);
bool simple_regex_search(std::string_view string, const boost::regex &regex);
void warn_match_in_check(std::string_view check_name, std::string_view matched_string, checks::level_t level);

} // namespace regex_utils
