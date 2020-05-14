#include "checks/checks.hpp"
#include "checks/o3.hpp"
#include <cppast/libclang_parser.hpp>
#include <fmt/format.h>
#include "diagnostic.hpp"

static void warn_if_more_than_5_functions(const cppast::cpp_file& parsed_file)
{
	size_t function_count = 0;
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
