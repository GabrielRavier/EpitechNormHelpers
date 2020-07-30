// SPDX-License-Identifier: GPL-3.0-or-later
#include "checks/f4.hpp"
#include "checks/checks.hpp"		  // for checks::level_t
#include "checks/stubs.hpp"			  // for checks::stubs::unimplementable
#include "diagnostic.hpp"			  // for diagnostic::warn
#include "managers.hpp"				  // for managers::resource_manager
#include <cppast/cpp_entity.hpp>	  // for cppast::cpp_entity
#include <cppast/cpp_entity_kind.hpp> // for cppast::is_function
#include <cppast/cpp_file.hpp>		  // for cppast::cpp_file
#include <cppast/visitor.hpp>		  // for cppast::visit, cppast::visitor_info
#include <cstddef>					  // for std::size_t
#include <fmt/core.h>				  // for fmt::format

// Level 1 checks for functions containing more than 20 statements (TODO, I have absolutely no idea of how to do this with cppast and there's a pretty good chance it's just impossible)
[[maybe_unused]] static void do_level1_one_file(const cppast::cpp_file &parsed_file)
{
	cppast::visit(
		parsed_file,
		[](const cppast::cpp_entity &entity) {
			return cppast::is_function(entity.kind());
		},
		[]([[maybe_unused]] const cppast::cpp_entity &entity, [[maybe_unused]] const cppast::visitor_info &information) {

		});
}

void checks::f4::do_check(checks::level_t, [[maybe_unused]] managers::resources_manager &check_resource_manager)
{
	// Attempted implementation that doesn't work for now
	/*
	 * auto parsed_files_list = check_resource_manager.cppast.request_parsed_files_list();
	 * std::for_each(parsed_files_list.begin(), parsed_files_list.end(), do_level1_one_file);
	 */
	checks::stubs::unimplementable("f4");
}
