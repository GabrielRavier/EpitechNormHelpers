// SPDX-License-Identifier: GPL-3.0-or-later
#include "options.hpp"
#include "checks/checks.hpp" // for checks::list
#include <algorithm>		 // for std::find_if
#include <cctype>			 // for stds::tolower
#include <cstdlib>			 // for std::exit, EXIT_SUCCESS
#include <cxxopts.hpp>		 // for cxxopts::Options, cxxopts::ParseResult, etc.
#include <fmt/core.h>		 // for fmt::format
#include <iostream>			 // for std::cout
#include <string>			 // for std::string
#include <unordered_map>	 // for std::unordered_map

/**
 * @brief From an option list, make a cxxopts::Options
 */
static void make_options_from_check_list(cxxopts::Options &options, const checks::list &check_list)
{
	for (const auto &check_category : check_list.categories)
	{
		char category_abbreviation_lowered = std::tolower(check_category.abbreviation);

		options.add_options()(fmt::format("check-{}all", category_abbreviation_lowered), fmt::format("Enable all checks in '{}' category with specified level", check_category.name), cxxopts::value<unsigned>()->implicit_value("1"));

		for (auto check_information : check_category.checks_information)
		{
			auto option_name_string = fmt::format("check-{}", check_information.short_name);
			auto description_string = fmt::format("Enable '{}' check ", check_information.name);
			if (check_information.maximum_level > 1)
				description_string += fmt::format("(levels 1 through {})", check_information.maximum_level);
			else
				description_string += "(level 1 only)";

			options.add_options()(option_name_string, description_string, cxxopts::value<unsigned>()->implicit_value("1"));
		}
	}
}

/**
 * @brief From a cxxopts::ParseResult and a checks::list, make a options_parser::parsed_options
 */
static options_parser::parsed_options make_parsed_options_from_parse_result(const cxxopts::ParseResult &parse_result, const checks::list &check_list)
{
	std::unordered_map<std::string, options_parser::parsed_check_option> result_map;

	for (const auto &argument : parse_result.arguments())
	{
		auto key = argument.key();
		if (key.compare(0, 6, "check-", 6) == 0)
		{
			// Got "check" argument
			auto checkArg = key.substr(6);

			if (checkArg == "all")
			{
				for (const auto &category : check_list.categories)
					for (const auto &check : category.checks_information)
						result_map[check.short_name] = {check, argument.as<unsigned>()};
			}
			else if (checkArg.substr(1) == "all")
			{
				char category_abbreviation = checkArg[0];
				auto corresponding_category = std::find_if(check_list.categories.begin(), check_list.categories.end(), [category_abbreviation](const auto &category) {
					return std::tolower(category.abbreviation) == category_abbreviation;
				});
				if (corresponding_category != check_list.categories.end())
					for (const auto &check : corresponding_category->checks_information)
						result_map[check.short_name] = {check, argument.as<unsigned>()};
			}
			else
			{
				char category_abbreviation = checkArg[0];
				auto corresponding_category = std::find_if(check_list.categories.begin(), check_list.categories.end(), [category_abbreviation](const auto &category) {
					return std::tolower(category.abbreviation) == category_abbreviation;
				});

				if (corresponding_category != check_list.categories.end())
					for (const auto &check : corresponding_category->checks_information)
						if (check.short_name == checkArg)
						{
							result_map[check.short_name] = {check, argument.as<unsigned>()};
							break;
						}
			}
		}
	}

	options_parser::parsed_options result;
	result.enabled_checks.reserve(result_map.size());

	for (auto elem : result_map)
		result.enabled_checks.push_back(elem.second);

	return result;
}

options_parser::parsed_options options_parser::parse_options(int argc, char *argv[])
{
	cxxopts::Options options{"epitech-norm-helper"};
	const checks::list &global_check_list = checks::get_global_check_list();

	make_options_from_check_list(options, global_check_list);

	options.add_options()
			("check-all", "Enable all checks at the specified level", cxxopts::value<unsigned>()->implicit_value("1"))
			("directory", "Directory in which the tool will run. All other paths are relative to this one", cxxopts::value<std::string>()->default_value("."))
			("compile-commands-directory", "Directory containing a compile_commands.json file", cxxopts::value<std::string>()->default_value("."))
			("h,help", "Print usage");

	options.parse_positional({"directory"});
	options.show_positional_help();
	options.positional_help("[DIRECTORY]...");

	auto parse_result = options.parse(argc, argv);

	if (parse_result.count("help"))
	{
		std::cout << options.help();
		std::exit(EXIT_SUCCESS);
	}

	auto parsed_options = make_parsed_options_from_parse_result(parse_result, global_check_list);
	parsed_options.directory = parse_result["directory"].as<std::string>();
	parsed_options.compile_commands_directory = parse_result["compile-commands-directory"].as<std::string>();

	return parsed_options;
}
