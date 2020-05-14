// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include <vector>
#include <array>
#include <functional>
#include <string>
#include "managers.hpp"

namespace checks
{

using level_t = unsigned char;

struct information
{
	const char *name;
	level_t maximum_level;
	std::function<void(level_t level, managers::resources_manager& check_resource_manager)> implementation;
	std::string short_name;
};

struct category
{
	std::vector<information> checks_information;
	const char *name;
	char abbreviation;
};

struct list
{
	std::array<category, 8> categories;
};

const list& get_global_check_list();

}
