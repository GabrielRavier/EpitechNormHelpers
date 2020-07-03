// SPDX-License-Identifier: GPL-3.0-or-later
#include "program.hpp"
#include "checks/checks.hpp" // for checks::information
#include "diagnostic.hpp"	 // for diagnostic::error
#include "managers.hpp"		 // for managers::resources_manager
#include "options.hpp"		 // for options_parser::parsed_options
#include <errno.h>			 // for errno
#include <exception>		 // for std::exception
#include <filesystem>		 // for std::filesystem::path
#include <fmt/core.h>		 // for fmt::format

void program(const options_parser::parsed_options &options)
{
	std::filesystem::current_path(options.directory);

	managers::resources_manager check_resource_manager{.cppast = {options.compile_commands_directory}};

	for (const auto &check : options.enabled_checks)
	{
		if (check.level < 1)
			continue;	// If the check is not at least at level 1, it is disabled

		auto check_implementation = check.check_information.implementation;
		if (check_implementation)
		{
			try
			{
				check_implementation(check.level, check_resource_manager);
			}
			catch (const std::exception &exception)
			{
				diagnostic::error(fmt::format("exception thrown while doing {} check : {}", check.check_information.short_name, exception.what()));
			}
		}
		else
		{
			diagnostic::error(fmt::format("{} check unimplemented", check.check_information.short_name));
		}
	}
}
