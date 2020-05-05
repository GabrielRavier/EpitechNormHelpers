#include "options.hpp"
#include <cxxopts.hpp>
#include <fmt/format.h>
#include <cppitertools/enumerate.hpp>
#include <array>
#include <string>
#include <vector>

namespace
{

struct rule_option
{
	const char *name;
	unsigned maximum_level;
};

struct option_category
{
	std::vector<rule_option> options;
	const char *name;
	char abbreviation;
};

struct option_list
{
	std::array<option_category, 8> categories;
};

}

/**
 * @brief From an option list, make a cxxopts::Options and a vector of strings containing all possible check options in abbreviated form ("o2" for "--check-o2")
 */
static void make_options_from_possible_options(const option_list& option_list, cxxopts::Options& options, std::vector<std::string>& checks_names)
{
	for (const auto& category : option_list.categories)
	{
		char category_abbreviation_lowered = tolower(category.abbreviation);

		options.add_options()(fmt::format("check-{}all", category_abbreviation_lowered), fmt::format("Enable all checks in '{}' category", category.name), cxxopts::value<unsigned>()->implicit_value("1"));

		for (auto [index, option] : iter::enumerate(category.options))
		{
			std::string check_name_string = fmt::format("{}{}", category_abbreviation_lowered, std::to_string(index));
			checks_names.push_back(check_name_string);

			std::string option_name_string = fmt::format("check-{}", check_name_string);

			options.add_options()(option_name_string, fmt::format("Enable '{}' check", option.name), cxxopts::value<unsigned>()->implicit_value("1"));
		}
	}
}

/**
 * @brief Make a cxxopts::Options and a vector of strings from the global list of options
 */
static void make_check_options(cxxopts::Options& options, std::vector<std::string>& check_options)
{
	static const option_list possible_options =
	{
		.categories =
		{{
			 {
				 .options =
				 {
					 {
						 .name = "Contents of the delivery folder",
						 .maximum_level = 1,
					 },
					 {
						 .name = "File extensions",
						 .maximum_level = 1,
					 },
					 {
						 .name = "File coherence",
						 .maximum_level = 1,
					 },
					 {
						 .name = "Naming files and folders",
						 .maximum_level = 1,
					 },
				 },
				 .name = "Files organization",
				 .abbreviation = 'O',
			 },
			 {
				 .options =
				 {
					 {
						 .name = "File header",
						 .maximum_level = 1,
					 },
					 {
						 .name = "Seperation of functions",
						 .maximum_level = 1,
					 },
					 {
						 .name = "Indentation of pre-processor directives",
						 .maximum_level = 1,
					 },
					 {
						 .name = "Global variables",
						 .maximum_level = 1,
					 },
					 {
						 .name = "Static",
						 .maximum_level = 1,
					 },
				 },
				 .name = "Global scope",
				 .abbreviation = 'G',
			 },
			 {
				 .options =
				 {
					 {
						 .name = "Coherence of functions",
						 .maximum_level = 1,
					 },
					 {
						 .name = "Naming functions",
						 .maximum_level = 1,
					 },
					 {
						 .name = "Number of columns",
						 .maximum_level = 1,
					 },
					 {
						 .name = "Number of lines",
						 .maximum_level = 1,
					 },
					 {
						 .name = "Arguments",
						 .maximum_level = 1,
					 },
					 {
						 .name = "Comments inside a function",
						 .maximum_level = 1,
					 },
					 {
						 .name = "Nested function",
						 .maximum_level = 1,
					 },
				 },
				 .name = "Functions",
				 .abbreviation = 'F',
			 },
			 {
				 .options =
				 {
					 {
						 .name = "Code line content",
						 .maximum_level = 1,
					 },
					 {
						 .name = "Indentation",
						 .maximum_level = 1,
					 },
					 {
						 .name = "Spaces",
						 .maximum_level = 1,
					 },
					 {
						 .name = "Curly brackets",
						 .maximum_level = 1,
					 },
					 {
						 .name = "Variable declaration",
						 .maximum_level = 1,
					 },
					 {
						 .name = "Line jumps",
						 .maximum_level = 1,
					 },
				 },
				 .name = "Layout inside a function scope",
				 .abbreviation = 'L',
			 },
			 {
				 .options =
				 {
					 {
						 .name = "Conditional branching",
						 .maximum_level = 1,
					 },
					 {
						 .name = "Ternary",
						 .maximum_level = 1,
					 },
					 {
						 .name = "Goto",
						 .maximum_level = 1,
					 },
				 },
				 .name = "Control structures",
				 .abbreviation = 'C',
			 },
			 {
				 .options =
				 {
					 {
						 .name = "Naming identifiers",
						 .maximum_level = 1,
					 },
					 {
						 .name = "Structures",
						 .maximum_level = 1,
					 },
					 {
						 .name = "Pointers",
						 .maximum_level = 1,
					 },
				 },
				 .name = "Variables and types",
				 .abbreviation = 'V',
			 },
			 {
				 .options =
				 {
					 {
						 .name = "Constant pointers",
						 .maximum_level = 1,
					 },
					 {
						 .name = "Typing",
						 .maximum_level = 1,
					 },
				 },
				 .name = "Advanced",
				 .abbreviation = 'A',
			 },
			 {
				 .options =
				 {
					 {
						 .name = "Content",
						 .maximum_level = 1,
					 },
					 {
						 .name = "Include guard",
						 .maximum_level = 1,
					 },
					 {
						 .name = "Macros",
						 .maximum_level = 1,
					 },
				 },
				 .name = "Header files",
				 .abbreviation = 'H',
			 }
		}}
	};

	make_options_from_possible_options(possible_options, options, check_options);
}

/**
 * @brief From a cxxopts::ParseResult and a vector of strings containing shortened versions of all possible options, make an options_parser::parsed_options
 */
static options_parser::parsed_options make_parsed_options_from_parse_result(const cxxopts::ParseResult& parse_result, const std::vector<std::string>& checks_names)
{
	options_parser::parsed_options result;

	for (const auto& argument : parse_result.arguments())
	{
		std::string key = argument.key();
		if (key.compare(0, 6, "check-", 6) == 0)
		{
			// Got "check" argument
			std::string checkArg = key.substr(6);

			if (checkArg == "all")
			{
				for (const std::string& check_name : checks_names)
					result.rule_options[check_name] = argument.as<unsigned>();
			}
			else if (checkArg.substr(1) == "all")
			{
				char category_abbreviation = checkArg[0];
				for (const std::string& check_name : checks_names)
					if (check_name[0] == category_abbreviation)
						result.rule_options[check_name] = argument.as<unsigned>();
			}
			else
			{
				result.rule_options[checkArg] = argument.as<unsigned>();
			}
		}
	}

	return result;
}

options_parser::parsed_options options_parser::parse_options(int argc, char *argv[])
{
	cxxopts::Options options("epitech-norm-helper");
	std::vector<std::string> checks_names;

	make_check_options(options, checks_names);

	options.add_options()
			("check-all", "All checks at the specified level", cxxopts::value<unsigned>())
			("directory", "Directory in which the tool will run", cxxopts::value<std::string>())
			("h,help", "Print usage");

	options.parse_positional({"directory"});
	options.show_positional_help();
	options.positional_help("[DIRECTORY]...");

	auto parse_result = options.parse(argc, argv);

	if (parse_result.count("help"))
	{
		std::cout << options.help();
		exit(EXIT_SUCCESS);
	}

	return make_parsed_options_from_parse_result(parse_result, checks_names);
}
