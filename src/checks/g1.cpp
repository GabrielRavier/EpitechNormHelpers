// SPDX-License-Identifier: GPL-3.0-or-later
#include "checks/g1.hpp"
#include "checks/checks.hpp"
#include "basename.hpp"
#include "diagnostic.hpp"
#include "file-utils.hpp"
#include "managers.hpp"
#include "regex-utils.hpp"
#include <boost/regex.hpp>
#include <fmt/format.h>
#include <algorithm>

static void check_beginning_of_files_for_string(const git::index::file_list& filenames, std::string_view start_check_string, std::string_view start_check_string_description, checks::level_t level)
{
	for (std::string_view filename : filenames)
	{
		const auto start_of_file = file_utils::attempt_file_to_string_with_size(filename, start_check_string.length());
		if (!start_of_file.has_value())
			continue;

		if (*start_of_file != start_check_string)
			diagnostic::warn(fmt::format("g1: level {}: Didn't find string at start of '{}' (string is \"{}\")", level, filename, start_check_string_description));
	}
}

static void check_files_for_regex(const git::index::file_list& filenames, const boost::regex& regex, std::string_view thing_searched, checks::level_t level)
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
static void do_level1(const git::index::file_list& filenames)
{
	static const boost::regex comment_regex{R"delimiter(\A[ \t\n]*(?:\/\*|\/\/))delimiter"};
	check_files_for_regex(filenames, comment_regex, "comment", 1);
}

// Level 2 checks for /* at the very beginning of the file
static void do_level2(const git::index::file_list& filenames)
{
	check_beginning_of_files_for_string(filenames, "/*", R"delimiter(/*)delimiter", 2);
}

// Level 3 checks for "/*\n**" at the very beginning of the file
static void do_level3(const git::index::file_list& filenames)
{
	check_beginning_of_files_for_string(filenames, "/*\n**", R"delimiter(/*\n**)delimiter", 3);
}

// Level 4 checks for "/*\n** EPITECH PROJECT, " at the very beginning of the file
static void do_level4(const git::index::file_list& filenames)
{
	check_beginning_of_files_for_string(filenames, "/*\n** EPITECH PROJECT, ", R"delimiter(/*\n** EPITECH PROJECT, )delimiter", 4);
}

// Level 5 checks for the full standard header of the school.
static void do_level5(const git::index::file_list& filenames)
{
	static const boost::regex header_regex{R"delimiter(\A/*\n\*\* EPITECH PROJECT, .*\n\*\* .*\n\*\* File description:\n\*\* .*\n*/)delimiter"};
	check_files_for_regex(filenames, header_regex, "Epitech header", 5);
}

// Level 6 is the same as level 5, except it also checks $YEAR for being a valid 4-digit number
static void do_level6(const git::index::file_list& filenames)
{
	static const boost::regex header_regex{R"delimiter(\A/*\n\*\* EPITECH PROJECT, \d{4}\n\*\* .*\n\*\* File description:\n\*\* .*\n*/)delimiter"};
	check_files_for_regex(filenames, header_regex, "Epitech header", 5);
}

// All the checks are only operated over .c, .cpp, .h and .hpp files
void checks::g1::do_check(checks::level_t level, managers::resources_manager& check_resource_manager)
{
	auto filenames = check_resource_manager.cwd_git.request_file_list();

	// Remove all files that aren't *.c, *.cpp, *.h or *.hpp files
	std::remove_if(filenames.begin(), filenames.end(), [](const auto& filename)
	{
		static const boost::regex basename_regex{R"delimiter(.*\.(?:[ch](?:pp)?))delimiter"};
		const auto basename = basename_wrappers::base_name(filename);

		boost::smatch match;
		return !bool{boost::regex_match(basename, match, basename_regex)};
	});

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
