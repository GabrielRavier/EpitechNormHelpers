// SPDX-License-Identifier: GPL-3.0-or-later
#include "checks/o3.hpp"
#include "checks/checks.hpp"
#include "diagnostic.hpp"
#include <cppast/cpp_entity_kind.hpp>
#include <cppast/libclang_parser.hpp>
#include <cppast/visitor.hpp>
#include <fmt/format.h>

// Level 1 checks for files containing more than 5 functions
static void do_level1_one_file(const cppast::cpp_file &parsed_file)
{
	size_t function_count = 0;
	cppast::visit(parsed_file, [&function_count](const cppast::cpp_entity &entity, cppast::visitor_info) {
		if (cppast::is_function(entity.kind()))
			function_count++;
	});

	if (function_count > 5)
		diagnostic::warn(fmt::format("o3: {}: {} functions found (more than 5), you may want to subdivide your file into several sub-files", parsed_file.name(), function_count));
}

void checks::o3::do_check(checks::level_t level, managers::resources_manager &check_resource_manager)
{
	auto parsed_files_list = check_resource_manager.cppast.request_parsed_files_list();
	if (level >= 1)
	{
		for (const auto &parsed_file : parsed_files_list)
			do_level1_one_file(parsed_file);
	}
}
