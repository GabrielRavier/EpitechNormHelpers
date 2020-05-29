// SPDX-License-Identifier: GPL-3.0-or-later
#include "checks/f4.hpp"
#include "checks/checks.hpp"		  // for checks::level_t
#include "diagnostic.hpp"			  // for diagnostic::warn
#include "managers.hpp"				  // for managers::resource_manager
#include <cppast/cpp_entity.hpp>	  // for cppast::cpp_entity
#include <cppast/cpp_entity_kind.hpp> // for cppast::is_function
#include <cppast/cpp_file.hpp>		  // for cppast::cpp_file
#include <cppast/visitor.hpp>		  // for cppast::visit, cppast::visitor_info
#include <cstddef>					  // for std::size_t
#include <fmt/core.h>				  // for fmt::format

// Level 1 checks for functions containing more than 20 statements
static void do_level1_one_file(const cppast::cpp_file &parsed_file)
{
	cppast::visit(parsed_file,
	[](const cppast::cpp_entity& entity) {
		return cppast::is_function(entity.kind());
	},
	[](const cppast::cpp_entity& entity, const cppast::visitor_info& information) {
	}
	);
}

void checks::f4::do_check(checks::level_t, managers::resources_manager &check_resource_manager)
{
	auto parsed_files_list = check_resource_manager.cppast.request_parsed_files_list();
	for (const auto &parsed_file : parsed_files_list)
		do_level1_one_file(parsed_file);
}
