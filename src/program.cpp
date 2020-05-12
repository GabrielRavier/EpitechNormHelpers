// SPDX-License-Identifier: GPL-3.0-or-later
#include "program.hpp"
#include "diagnostic.hpp"
#include "checks/checks.hpp"
#include <iostream>
#include <string_view>
#include <fmt/format.h>
#include <unistd.h>
#include <system_error>

static void change_current_directory(const std::string& directory)
{
	int result = chdir(directory.c_str());
	if (result != 0)
		throw std::system_error(errno, std::generic_category(), fmt::format("invalid directory '{}' given", directory));
}

static auto make_rules_functions_map()
{
	std::unordered_map<std::string, std::function<void(int check_level)>> result;
	result["o1"] = checks::o1;
	result["o2"] = checks::o2;
	result["o3"] = checks::o3;

	return result;
}

void program(const options_parser::parsed_options& options)
{
	change_current_directory(options.directory);

	auto rules_functions_map = make_rules_functions_map();
	for (const auto& rule : options.rule_options)
	{
		auto match = rules_functions_map.find(rule.first);
		if (match != rules_functions_map.end())
		{
			try
			{
				match->second(rule.second);
			}
			catch (const std::exception& exception)
			{
				diagnostic::error(fmt::format("exception thrown while doing {} check : {}", rule.first, exception.what()));
			}
		}
		else
		{
			diagnostic::error(fmt::format("{} check unimplemented", rule.first));
		}
	}
}
