// SPDX-License-Identifier: GPL-3.0-or-later
#include "regex-utils.hpp"
#include <string>
#include <boost/regex.hpp>

std::string regex_utils::escape_string_for_insertion_in_regex(const std::string& string)
{
	static const boost::regex escape_regex{R"delimiter([.^$|()\[\]{}*+?\\])delimiter"};
	constexpr const char *replace_regex{R"delimiter(\\&)delimiter"};
	return boost::regex_replace(string, escape_regex, replace_regex, boost::match_default | boost::format_sed);
}
