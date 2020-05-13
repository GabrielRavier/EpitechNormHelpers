// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include <vector>
#include <array>
#include <functional>
#include <string>

namespace checks
{

using level_t = unsigned char;

void o1(level_t level);
void o2(level_t level);
void o3(level_t level);

enum flag : unsigned char
{
	flag_none = 0,
	flag_uses_cppast = 1 << 0,
	flag_uses_libgit = 1 << 1,
};

struct information
{
	const char *name;
	level_t maximum_level;
	enum flag flags;
	std::function<void(int level)> implementation;
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
