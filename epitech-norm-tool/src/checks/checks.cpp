// SPDX-License-Identifier: GPL-3.0-or-later
#include "checks/checks.hpp"
#include "checks/g1.hpp"
#include "checks/g2.hpp"
#include "checks/o1.hpp"
#include "checks/o2.hpp"
#include "checks/o3.hpp"
#include "checks/o4.hpp"
#include <cctype>
#include <enumerate.hpp>
#include <fmt/format.h>

static auto get_unfinished_global_check_list()
{
	static const checks::list check_list =
		{
			.categories =
				{{{
					  .checks_information =
						  {
							  {
								  .name = "Contents of the delivery folder",
								  .maximum_level = 3,
								  .implementation = checks::o1::do_check,
							  },
							  {
								  .name = "File extensions",
								  .maximum_level = 5,
								  .implementation = checks::o2::do_check,
							  },
							  {
								  .name = "File coherence",
								  .maximum_level = 1,
								  .implementation = checks::o3::do_check,
							  },
							  {
								  .name = "Naming files and folders",
								  .maximum_level = 2,
								  .implementation = checks::o4::do_check,
							  },
						  },
					  .name = "Files organization",
					  .abbreviation = 'O',
				  },
				  {
					  .checks_information =
						  {
							  {
								  .name = "File header",
								  .maximum_level = 1,
								  .implementation = checks::g1::do_check,
							  },
							  {
								  .name = "Seperation of functions",
								  .maximum_level = 1,
								  .implementation = checks::g2::do_check,
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
					  .checks_information =
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
					  .checks_information =
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
					  .checks_information =
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
					  .checks_information =
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
					  .checks_information =
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
					  .checks_information =
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
				  }}}};

	return check_list;
}

static auto finish_check_list(checks::list list)
{
	for (auto &category : list.categories)
	{
		char category_abbreviation_lowered = std::tolower(category.abbreviation);

		size_t index = 1;
		for (auto &check : category.checks_information)
		{
			check.short_name = fmt::format("{}{}", category_abbreviation_lowered, index);
			++index;
		}
	}

	return list;
}

const checks::list &checks::get_global_check_list()
{
	// Can't do this at compile time yet :(, maybe if I ever update this to use C++20
	static auto global_check_list = finish_check_list(get_unfinished_global_check_list());
	return global_check_list;
}
