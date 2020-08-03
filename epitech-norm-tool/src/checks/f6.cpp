// SPDX-License-Identifier: GPL-3.0-or-later
#include "checks/f6.hpp"
#include "managers.hpp"		// for managers::resources_manager
#include "diagnostic.hpp"	// for diagnostic::warn
#include "file-utils.hpp"	// for file_utils::attempt_file_to_string
#include <boost/regex.hpp>	// for boost::regex
#include <fmt/core.h>		// for fmt::format
#include <iterator>			// for std::distance
#include <string_view>		// for std::string_view

static void match_all_occurences_of_regex_in_files(const git::index::file_list &filenames, const boost::regex& comment_regex, int level)
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
		while (boost::regex_search(file_contents_sv.cbegin(), file_contents_sv.cend(), match, comment_regex))
		{
			// Thanks C++17 for forcing me to do this... At least C++20 lets you just pass it two iterators and have it work
			auto matched_string = std::string_view{match[0].first, static_cast<size_t>(std::distance(match[0].first, match[0].second))};
			auto match_offset = std::distance(original_file_contents_sv.begin(), match[0].first);	// Use original_file_contents_sv to get the actual offset from the file that we actually use
			diagnostic::warn(fmt::format("f6:{}: comment '{}' at offset {} matched level {}", filename, matched_string, match_offset, level));

			auto match_end_offset_from_current = std::distance(file_contents_sv.begin(), match[0].second);
			file_contents_sv.remove_prefix(match_end_offset_from_current);	// The next scan will be done on the file's contents, but starting at the end of the match we got
		}
	}
}

// Level 1 checks for any line in the file that has a comment that is preceded by a space or a tab
static void do_level1(const git::index::file_list &filenames)
{
	// [ \t] matches a space or a tab
	// \/[*\/] matches // or /*
	// This regex is pretty shit because it will even match something like `// //` because the second `//` is not at the beginning of the string and has a space before it
	static const boost::regex comment_regex{R"delimiter([ \t]\/[*\/])delimiter"};

	match_all_occurences_of_regex_in_files(filenames, comment_regex, 1);
}

// Level 2 checks for any line in the file that has a comment that does not start on the first character
static void do_level2(const git::index::file_list &filenames)
{
	// (?<!^) asserts that we are not at the beginning of a file
	// \/[*\/] matches // or /*
	// This regex is pretty shit because it will even match something like `///` because the two lasts characters are `//` and are not at the beginning of the line
	static const boost::regex comment_regex{R"delimiter((?<!^)\/[*\/])delimiter"};

	match_all_occurences_of_regex_in_files(filenames, comment_regex, 1);
}

void checks::f6::do_check(checks::level_t level, managers::resources_manager &check_resource_manager)
{
	const auto &filenames = check_resource_manager.cwd_git.request_c_cpp_source_file_list();

	if (level == 1)
		do_level1(filenames);
	else if (level >= 2)
		do_level2(filenames);
}
