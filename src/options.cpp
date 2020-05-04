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

static void make_options_from_possible_options(cxxopts::Options& options, const option_list& option_list)
{
	for (const auto& category : option_list.categories)
	{
		char category_abbreviation_lowered = tolower(category.abbreviation);
		for (auto [index, option] : iter::enumerate(category.options))
		{
			std::string option_name_string = fmt::format("check-{}{}", category_abbreviation_lowered, std::to_string(index));

			if (option.maximum_level > 1)
				options.add_options()(option_name_string, fmt::format("{}", option.name), cxxopts::value<unsigned>());
			else
				options.add_options()(option_name_string, fmt::format("{}", option.name));
		}
	}
}

static void make_check_options(cxxopts::Options& options)
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

	make_options_from_possible_options(options, possible_options);
}

options_parser::parsed_options options_parser::parse_options(int argc, char *argv[])
{
	cxxopts::Options options("epitech-norm-helper");

	make_check_options(options);

	options.add_options()
			("check-all", "All checks at the specified level", cxxopts::value<unsigned>())
			("h,help", "Print usage");

	options.parse_positional({"directory"});
	options.show_positional_help();
	options.positional_help("[DIRECTORY]...");

	auto parsedOptions = options.parse(argc, argv);

	if (parsedOptions.count("help"))
	{
		std::cout << options.help();
		exit(EXIT_SUCCESS);
	}

	return {};
}
