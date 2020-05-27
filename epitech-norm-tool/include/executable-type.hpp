// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include <fmt/core.h>	// for fmt::format_parse_context
#include <fmt/format.h>	// for fmt::formatter
#include <filesystem>	// for std::filesystem::path
#include <fstream>	// for std::ifstream
#include <string_view>	// std::string_view

namespace executable
{

enum class type
{
	none,
	elf,
	mz,
	dalvik,
};

executable::type get_type_from_stream(std::ifstream &file);
executable::type get_type_from_file(std::filesystem::path path);

} // namespace executable

template <>
struct fmt::formatter<executable::type> : fmt::formatter<std::string_view>
{
	constexpr auto parse(fmt::format_parse_context &context)
	{
		return context.begin();
	}

	template <typename FormatContext> auto format(const executable::type &executable_type, FormatContext &context)
	{
		std::string_view name;
		switch (executable_type)
		{
		case executable::type::none:
			name = "None";
			break;

		case executable::type::elf:
			name = "ELF";
			break;

		case executable::type::mz:
			name = "MZ and/or PE";
			break;

		case executable::type::dalvik:
			name = "Dalvik";
			break;
		}

		return fmt::formatter<std::string_view>::format(name, context);
	}
};
