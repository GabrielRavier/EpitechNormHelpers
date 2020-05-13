// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include <string>
#include <vector>
#include <utility>
#include <tuple>
#include <unordered_set>
#include <functional>
#include <filesystem>
#include "checks/checks.hpp"

namespace options_parser
{
	struct parsed_check_option
	{
		checks::information check_information;
		unsigned level;
	};

	struct parsed_options
	{
		std::vector<parsed_check_option> enabled_checks;
		std::string directory;
		std::filesystem::path compile_commands_directory;
	};

	/**
	 * @brief From the arguments normally passed to main, form an options_parser::parsed_options
	 */
	parsed_options parse_options(int argc, char *argv[]);
}
