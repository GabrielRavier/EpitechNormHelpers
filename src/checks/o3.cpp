#include "checks/checks.hpp"
#include "checks/o3.hpp"
#include <cppast/libclang_parser.hpp>
#include <fmt/format.h>
#include "diagnostic.hpp"

static void do_level1()
{
}

void checks::o3::do_check(checks::level_t level, managers::resources_manager& check_resource_manager)
{
	check_resource_manager.cppast.request_parsed_files_list();
	if (level >= 1)
		do_level1();
}
