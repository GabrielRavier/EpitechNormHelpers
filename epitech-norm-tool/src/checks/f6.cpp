// SPDX-License-Identifier: GPL-3.0-or-later
#include "checks/f6.hpp"
#include "managers.hpp"		// for managers::resources_manager
#include "diagnostic.hpp"	// for diagnostic::warn
#include "file-utils.hpp"	// for file_utils::attempt_file_to_string
#include "regex-utils.hpp"	// for regex_utils::match_all_occurences_of_regex_in_files
#include <boost/regex.hpp>	// for boost::regex
#include <fmt/core.h>		// for fmt::format
#include <iterator>			// for std::distance
#include <string_view>		// for std::string_view

// Level 1 checks for any line in the file that has a comment that is preceded by a space or a tab
static void do_level1(const git::index::file_list &filenames)
{
	// [ \t] matches a space or a tab
	// \/[*\/] matches // or /*
	// This regex is pretty shit because it will even match something like `// //` because the second `//` is not at the beginning of the string and has a space before it
	static const boost::regex comment_regex{R"delimiter([ \t]\/[*\/])delimiter"};

	regex_utils::match_all_occurences_of_regex_in_files(filenames, "comment", comment_regex, 1);
}

// Level 2 checks for any line in the file that has a comment that does not start on the first character
static void do_level2(const git::index::file_list &filenames)
{
	// (?<!^) asserts that we are not at the beginning of a file
	// \/[*\/] matches // or /*
	// This regex is pretty shit because it will even match something like `///` because the two lasts characters are `//` and are not at the beginning of the line
	static const boost::regex comment_regex{R"delimiter((?<!^)\/[*\/])delimiter"};

	regex_utils::match_all_occurences_of_regex_in_files(filenames, "comment", comment_regex, 2);
}

void checks::f6::do_check(checks::level_t level, managers::resources_manager &check_resource_manager)
{
	const auto &filenames = check_resource_manager.cwd_git.request_c_cpp_source_file_list();

	if (level == 1)
		do_level1(filenames);
	else if (level >= 2)
		do_level2(filenames);
}
