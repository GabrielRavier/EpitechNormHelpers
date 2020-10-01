// SPDX-License-Identifier: GPL-3.0-or-later
#include "checks/l1.hpp"
#include "managers.hpp"		// for managers::resources_manager
#include "diagnostic.hpp"	// for diagnostic::warn
#include "file-utils.hpp"	// for file_utils::attempt_file_to_string
#include "regex-utils.hpp"	// for regex_utils::match_all_occurences_of_regex_in_files
#include <boost/regex.hpp>	// for boost::regex
#include <fmt/core.h>		// for fmt::format
#include <iterator>			// for std::distance
#include <string_view>		// for std::string_view

// Level 1 checks for any line in the file that has multiple assignments or multiple statements (occurences of ;)
static void do_level1(const git::index::file_list &filenames)
{
	// (?:[+\-*\/%&|\^]|<<|>>)?= matches any assignment operator
	// We basically match having two properly formatted (properly spaced) assignment operators in the same line
	// We also match having two occurences of ; in the same line
	static const boost::regex comment_regex{R"delimiter((?: (?:[+\-*\/%&|\^]|<<|>>)?= |while|if|switch).* (?:[+\-*\/%&|\^]|<<|>>)?= |;.*;)delimiter"};

	// match_not_dot_newline is used to avoid getting ridiculous multi-line matches
	regex_utils::match_all_occurences_of_regex_in_files(filenames, "statements/assignments/conditions", 1, comment_regex, boost::regex_constants::match_flags::match_not_dot_newline);
}

void checks::l1::do_check(checks::level_t level, managers::resources_manager &check_resource_manager)
{
	diagnostic::warn("l1: This check is somewhat crude (and incomplete within the tool itself, you should use clang-format and clang-tidy to complete it) and may not work properly if your source files aren't properly formatted or if you have very long statements (that take multiple lines).");
	const auto &filenames = check_resource_manager.cwd_git.request_c_cpp_source_file_list();

	if (level >= 1)
		do_level1(filenames);
}
