#include "checks/checks.hpp"
#include "checks/o3.hpp"
#include <cppast/libclang_parser.hpp>
#include <cppast/visitor.hpp>
#include <cppast/cpp_entity_kind.hpp>
#include <fmt/format.h>
#include "diagnostic.hpp"

static void warn_if_more_than_5_functions(const cppast::cpp_file& parsed_file)
{
	size_t function_count = 0;
	cppast::visit(parsed_file, [&function_count](const cppast::cpp_entity& entity, cppast::visitor_info)
	{
		if (cppast::is_function(entity.kind()))
			function_count++;
	});

	if (function_count > 5)
		diagnostic::warn(fmt::format("o3: {}: {} functions found (more than 5), you may want to subdivide your file into several sub-files", parsed_file.name(), function_count));
}

void checks::o3::do_check(checks::level_t level, managers::resources_manager& check_resource_manager)
{
	auto parsed_files_list = check_resource_manager.cppast.request_parsed_files_list();
	if (level >= 1)
	{
		for (const auto& parsed_file : parsed_files_list)
			warn_if_more_than_5_functions(parsed_file);
	}
}
