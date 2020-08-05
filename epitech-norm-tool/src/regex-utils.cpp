// SPDX-License-Identifier: GPL-3.0-or-later
#include "regex-utils.hpp"
#include "file-utils.hpp"			// for file_utils::attempt_file_to_string
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

void regex_utils::match_all_occurences_of_regex_in_files(const git::index::file_list &filenames, std::string_view description_of_what_you_re_looking_for, const boost::regex& regex, int level)
{
	diagnostic::warn("f6: This check is very crude and may not work properly, especially if you have any kind of indentation outside of a function");

	for (std::string_view filename : filenames)
	{
		const auto file_contents = file_utils::attempt_file_to_string(filename);

		if (!file_contents.has_value())
			continue;

		auto file_contents_sv = std::string_view{*file_contents};
		auto original_file_contents_sv = file_contents_sv;	// Save this for offset calculations

		// Get all matches in the file and diagnose them (idk if I can even do that tbh)
		boost::match_results<decltype(file_contents_sv)::const_iterator> match;
		while (boost::regex_search(file_contents_sv.cbegin(), file_contents_sv.cend(), match, regex))
		{
			// Thanks C++17 for forcing me to do this... At least C++20 lets you just pass it two iterators and have it work
			auto matched_string = std::string_view{match[0].first, static_cast<size_t>(std::distance(match[0].first, match[0].second))};
			auto match_offset = std::distance(original_file_contents_sv.begin(), match[0].first);	// Use original_file_contents_sv to get the actual offset from the file that we actually use
			diagnostic::warn(fmt::format("f6:{}: {} '{}' at offset {} matched level {}", filename, description_of_what_you_re_looking_for, matched_string, match_offset, level));

			auto match_end_offset_from_current = std::distance(file_contents_sv.begin(), match[0].second);
			file_contents_sv.remove_prefix(match_end_offset_from_current);	// The next scan will be done on the file's contents, but starting at the end of the match we got
		}
	}
}
