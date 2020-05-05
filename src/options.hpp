#pragma once
#include <unordered_map>
#include <array>
#include <string>

namespace options_parser
{
	struct parsed_options
	{
		std::unordered_map<std::string, unsigned> rule_options;
		std::string directory;
	};

	/**
	 * @brief From the arguments normally passed to main, form an options_parser::parsed_options
	 */
	parsed_options parse_options(int argc, char *argv[]);
}
