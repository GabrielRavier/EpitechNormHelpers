// SPDX-License-Identifier: GPL-3.0-or-later
#include "checks/o4.hpp"
#include "checks/checks.hpp"
#include "managers.hpp"
#include "basename.hpp"
#include "diagnostic.hpp"
#include <boost/regex.hpp>
#include <fmt/format.h>

// Level 1 checks for *.c, *.h, *.cpp and *.hpp files that don't have a snake_case name
static void do_level1(const git::index::file_list& filenames)
{
	for (std::string_view filename : filenames)
	{
		static const boost::regex snake_case_source_code_regex{R"delimiter([^a-z0-9_\-].*\.(?:[ch]|[ch]pp))delimiter"};
		const std::string basename = basename_wrappers::base_name(filename);

		boost::smatch match;
		if (boost::regex_match(basename, match, snake_case_source_code_regex))
			diagnostic::warn(fmt::format("o4: '{}' matched level 1", filename));
	}
}

// Level 2 checks for filenames not having a snake_case name
static void do_level2(const git::index::file_list& filenames)
{
	for (std::string_view filename : filenames)
	{
		static const boost::regex snake_case_filename_regex{R"delimiter([^a-z0-9_\-\.\/])delimiter"};

		boost::match_results<std::string_view::const_iterator> sv_match;
		if (boost::regex_search(filename.begin(), filename.end(), sv_match, snake_case_filename_regex))
			diagnostic::warn(fmt::format("o4: '{}' matched level 2", filename));
	}
}

void checks::o4::do_check(checks::level_t level, managers::resources_manager& check_resource_manager)
{
	git::index::file_list filenames = check_resource_manager.cwd_git.request_file_list();

	if (level == 1)
		do_level1(filenames);
	else if (level >= 2)
		do_level2(filenames);
}
