// SPDX-License-Identifier: GPL-3.0-or-later
#include "regex-utils.hpp"
#include "diagnostic.hpp"			// for diagnostic::warn
#include <boost/regex/v4/regex.hpp> // for boost::regex
#include <fmt/core.h>				// for fmt::format
#include <string_view>				// for std::string_view

std::string regex_utils::escape_string_for_insertion_in_regex(std::string_view string)
{
	static const boost::regex escape_regex{R"delimiter([.^$|()\[\]{}*+?\\])delimiter"};
	constexpr const char *formatter{R"delimiter(\\&)delimiter"};

	std::string result;
	boost::regex_replace(std::back_inserter(result), string.cbegin(), string.cend(), escape_regex, formatter, boost::match_default | boost::format_sed);

	return result;
}

bool regex_utils::simple_regex_match(std::string_view string, const boost::regex &regex)
{
	return boost::regex_match(string.cbegin(), string.cend(), regex);
}

bool regex_utils::simple_regex_search(std::string_view string, const boost::regex &regex)
{
	return boost::regex_search(string.cbegin(), string.cend(), regex);
}

void regex_utils::warn_match_in_check(std::string_view check_name, std::string_view matched_string, checks::level_t level)
{
	diagnostic::warn(fmt::format("{}: '{}' matched level {}", check_name, matched_string, level));
}
