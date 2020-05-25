// SPDX-License-Identifier: GPL-3.0-or-later
#include "checks/o4.hpp"
#include "basename.hpp"
#include "checks/checks.hpp"
#include "diagnostic.hpp"
#include "managers.hpp"
#include "regex-utils.hpp"
#include <boost/regex.hpp>
#include <fmt/format.h>

static void warn_match(std::string_view matched_string, checks::level_t level)
{
	regex_utils::warn_match_in_check("o4", matched_string, level);
}

// Level 1 checks for *.c, *.h, *.cpp and *.hpp files that don't have a snake_case name
static void do_level1(const git::index::file_list &filenames)
{
	for (std::string_view filename : filenames)
	{
		static const boost::regex snake_case_source_code_regex{R"delimiter([^a-z0-9_\-].*\.(?:[ch]|[ch]pp)$)delimiter"};
		if (regex_utils::simple_regex_search(basename_wrappers::base_name(filename), snake_case_source_code_regex))
			warn_match(filename, 1);
	}
}

// Level 2 checks for filenames not having a snake_case name
static void do_level2(const git::index::file_list &filenames)
{
	for (std::string_view filename : filenames)
	{
		static const boost::regex snake_case_filename_regex{R"delimiter([^a-z0-9_\-\.\/])delimiter"};
		if (regex_utils::simple_regex_search(filename, snake_case_filename_regex))
			warn_match(filename, 2);
	}
}

void checks::o4::do_check(checks::level_t level, managers::resources_manager &check_resource_manager)
{
	git::index::file_list filenames = check_resource_manager.cwd_git.request_file_list();

	if (level == 1)
		do_level1(filenames);
	else if (level >= 2)
		do_level2(filenames);
}
