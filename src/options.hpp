#pragma once
#include <vector>
#include <array>

namespace options_parser
{
	struct norm_helper_option_category
	{
		std::vector<int> option_levels;
	};

	struct parsed_options
	{
		std::array<norm_helper_option_category, 8> option_categories;
	};

	parsed_options parse_options(int argc, char *argv[]);
}
