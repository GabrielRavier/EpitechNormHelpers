// SPDX-License-Identifier: GPL-3.0-or-later
#include "checks/o3.hpp"
#include "checks/checks.hpp"		  // for checks::level_t
#include "diagnostic.hpp"			  // for diagnostic::warn
#include "managers.hpp"				  // for managers::resource_manager
#include <cppast/cpp_entity.hpp>	  // for cppast::cpp_entity
#include <cppast/cpp_entity_kind.hpp> // for cppast::is_function
#include <cppast/cpp_file.hpp>		  // for cppast::cpp_file
#include <cppast/visitor.hpp>		  // for cppast::visit, cppast::visitor_info
#include <cstddef>					  // for std::size_t
#include <fmt/core.h>				  // for fmt::format

// Level 1 checks for files containing more than 5 functions
static void do_level1_one_file(const cppast::cpp_file &parsed_file)
{
	std::size_t function_count = 0;
	cppast::visit(parsed_file, [&function_count](const cppast::cpp_entity &entity, cppast::visitor_info) {
		if (cppast::is_function(entity.kind()))
			function_count++;
	});

	if (function_count > 5)
		diagnostic::warn(fmt::format("o3: {}: {} functions found (more than 5), you may want to subdivide your file into several sub-files", parsed_file.name(), function_count));
}

void checks::o3::do_check(checks::level_t, managers::resources_manager &check_resource_manager)
{
	auto parsed_files_list = check_resource_manager.cppast.request_parsed_files_list();
	std::for_each(parsed_files_list.begin(), parsed_files_list.end(), do_level1_one_file);
}
