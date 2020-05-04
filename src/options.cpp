#include "options.hpp"
#include <cxxopts.hpp>
#include <fmt/format.h>
#include <array>

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
	char category_abbreviation;
};

struct option_list
{
	std::array<option_category, 8> categories;
};

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
			 .category_abbreviation = 'O',
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
			 .category_abbreviation = 'G',
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
			 .category_abbreviation = 'F',
		 },
		 {
			 .options = {},
			 .name = "Layout inside a function scope",
			 .category_abbreviation = 'L',
		 },
		 {
			 .options = {},
			 .name = "Variables and types",
			 .category_abbreviation = 'V',
		 },
		 {
			 .options = {},
			 .name = "Control structures",
			 .category_abbreviation = 'C',
		 }
	}}
};

}

options_parser::parsed_options options_parser::parse_options(int argc, char *argv[])
{
	cxxopts::Options options("epitech-norm-helper", "- Helper for respecting Epitech norms");

	// Mostly test code, will scrap later
	constexpr const char *o_name = "Files organization";
	constexpr const char *g_name = "Global scope";

	options.add_options()
			("check-o1", fmt::format("{} - Contents of the delivery folder", o_name), cxxopts::value<unsigned>())
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

}
