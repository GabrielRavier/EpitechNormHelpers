// SPDX-License-Identifier: GPL-3.0-or-later
#include "program.hpp"
#include "diagnostic.hpp"
#include "managers.hpp"
#include "checks/checks.hpp"
#include <iostream>
#include <string_view>
#include <fmt/format.h>
#include <unistd.h>
#include <system_error>

static void change_current_directory(const std::filesystem::path& directory)
{
	int result = chdir(directory.c_str());
	if (result != 0)
		throw std::system_error(errno, std::generic_category(), fmt::format("invalid directory '{}' given", directory.string()));
}

void program(const options_parser::parsed_options& options)
{
	change_current_directory(options.directory);

	managers::resources_manager check_resource_manager {.cppast = {options.compile_commands_directory}};

	for (const auto& check : options.enabled_checks)
	{
		auto check_implementation = check.check_information.implementation;
		if (check_implementation)
		{
			try
			{
				check_implementation(check.level, check_resource_manager);
			}
			catch (const std::exception& exception)
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
