// SPDX-License-Identifier: GPL-3.0-or-later
#include "checks/f2.hpp"
#include "checks/checks.hpp"  // for checks::level_t
#include "diagnostic.hpp"	  // for diagnostic::error
#include "managers.hpp"		  // for managers::resources_manager
#include "regex-utils.hpp"	  // for regex_utils::simple_regex_search
#include <boost/regex.hpp>	  // for boost::regex
#include <cppast/visitor.hpp> // for cppast::visit, cppast::visitor_info
#include <fmt/core.h>		  // for fmt::format
#include <string_view>		  // for std::string_view

static bool is_snake_case(std::string_view string)
{
	static const boost::regex snake_case_name_regex{R"delimiter([a-z0-9_]*)delimiter"};
	return regex_utils::simple_regex_match(string, snake_case_name_regex);
}

// Level 1 checks for functions having a non-snake_case name
static void do_level1_one_file(const cppast::cpp_file &parsed_file)
{
	cppast::visit(parsed_file, [&parsed_file](const cppast::cpp_entity &entity, cppast::visitor_info) {
		if (cppast::is_function(entity.kind()))
			if (!is_snake_case(entity.name()))
				diagnostic::warn(fmt::format("f2: {}: '{}' function does not have a snake_case name", parsed_file.name(), entity.name()));
	});
}

void checks::f2::do_check(checks::level_t level, managers::resources_manager &check_resource_manager)
{
	auto parsed_files_list = check_resource_manager.cppast.request_parsed_files_list();
	if (level >= 1)
	{
		for (const auto &parsed_file : parsed_files_list)
			do_level1_one_file(parsed_file);
	}
}
