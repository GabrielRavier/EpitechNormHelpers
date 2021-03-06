// SPDX-License-Identifier: GPL-3.0-or-later
#include "checks/g1.hpp"
#include "basename.hpp"				// for basename_wrappers::base_name
#include "checks/checks.hpp"		// for checks::level_t
#include "diagnostic.hpp"			// for diagnostic::warn
#include "file-utils.hpp"			// for file_utils::attempt_file_to_string, file_utils::error, etc.
#include "libgit2wrapper/index.hpp" // for git::index
#include "managers.hpp"				// for managers::resources_manager
#include "regex-utils.hpp"			// for regex_utils::simple_regex_search
#include <algorithm>				// for std::remove_if
#include <boost/regex.hpp>			// for boost::regex
#include <fmt/core.h>				// for fmt::format
#include <string_view>				// for std::string_view
#include <variant>					// for std::holds_alternative

static void check_beginning_of_files_for_string(const git::index::file_list &filenames, std::string_view start_check_string, std::string_view start_check_string_description, checks::level_t level)
{
	for (std::string_view filename : filenames)
	{
		const auto start_of_file = file_utils::attempt_file_to_string_with_size(filename, start_check_string.length());
		if (std::holds_alternative<file_utils::error>(start_of_file) && std::get<file_utils::error>(start_of_file) != file_utils::error::too_small)
			continue;

		if (!std::holds_alternative<std::string>(start_of_file) || std::get<std::string>(start_of_file) != start_check_string)
			diagnostic::warn(fmt::format("g1: level {}: Didn't find string at start of '{}' (string is \"{}\")", level, filename, start_check_string_description));
	}
}

static void check_files_for_regex(const git::index::file_list &filenames, const boost::regex &regex, std::string_view thing_searched, checks::level_t level)
{
	for (std::string_view filename : filenames)
	{
		const auto file_contents = file_utils::attempt_file_to_string(filename);

		if (!file_contents.has_value())
			continue;

		if (!regex_utils::simple_regex_search(*file_contents, regex))
			diagnostic::warn(fmt::format("g1: level {}: Didn't find {} at start of '{}'", level, thing_searched, filename));
	}
}

// Level 1 checks for any comment before anything else (but allows them after any amount of whitespace or newlines before it)
static void do_level1(const git::index::file_list &filenames)
{
	static const boost::regex comment_regex{R"delimiter(\A[ \t\n]*(?:\/\*|\/\/))delimiter"};
	check_files_for_regex(filenames, comment_regex, "comment", 1);
}

// Level 2 checks for /* at the very beginning of the file
static void do_level2(const git::index::file_list &filenames)
{
	check_beginning_of_files_for_string(filenames, "/*", R"delimiter(/*)delimiter", 2);
}

// Level 3 checks for "/*\n**" at the very beginning of the file
static void do_level3(const git::index::file_list &filenames)
{
	check_beginning_of_files_for_string(filenames, "/*\n**", R"delimiter(/*\n**)delimiter", 3);
}

// Level 4 checks for "/*\n** EPITECH PROJECT, " at the very beginning of the file
static void do_level4(const git::index::file_list &filenames)
{
	check_beginning_of_files_for_string(filenames, "/*\n** EPITECH PROJECT, ", R"delimiter(/*\n** EPITECH PROJECT, )delimiter", 4);
}

// Level 5 checks for the full standard header of the school.
static void do_level5(const git::index::file_list &filenames)
{
	static const boost::regex header_regex{R"delimiter(\A/\*\n\*\* EPITECH PROJECT, .*\n\*\* .*\n\*\* File description\:\n\*\* .*\n\*/)delimiter"};
	check_files_for_regex(filenames, header_regex, "Epitech header", 5);
}

// Level 6 is the same as level 5, except it also checks $YEAR for being a valid 4-digit number
static void do_level6(const git::index::file_list &filenames)
{
	static const boost::regex header_regex{R"delimiter(\A/\*\n\*\* EPITECH PROJECT, [0-9]{4}\n\*\* .*\n\*\* File description\:\n\*\* .*\n\*/)delimiter"};
	check_files_for_regex(filenames, header_regex, "Epitech header with valid year", 5);
}

// All the checks are only operated over .c, .cpp, .h and .hpp files
void checks::g1::do_check(checks::level_t level, managers::resources_manager &check_resource_manager)
{
	const auto &filenames = check_resource_manager.cwd_git.request_c_cpp_source_file_list();

	if (level == 1)
		do_level1(filenames);
	else if (level == 2)
		do_level2(filenames);
	else if (level == 3)
		do_level3(filenames);
	else if (level == 4)
		do_level4(filenames);
	else if (level == 5)
		do_level5(filenames);
	else if (level >= 6)
		do_level6(filenames);
}
