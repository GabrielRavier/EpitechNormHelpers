// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include <filesystem>	// for std::filesystem::path
#include <fmt/core.h>	// for fmt::format_parse_context
#include <fmt/format.h> // for fmt::formatter
#include <fstream>		// for std::ifstream
#include <string_view>	// std::string_view

namespace executable
{

enum class type : char
{
	not_an_executable_or_unknown,
	elf,
	mz_and_or_pe,	// It may be either a DOS or a Windows executable (or both)
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
		using namespace std::string_view_literals;
		std::string_view name;
		switch (executable_type)
		{
		case executable::type::not_an_executable_or_unknown:
			name = "None"sv;
			break;

		case executable::type::elf:
			name = "ELF"sv;
			break;

		case executable::type::mz_and_or_pe:
			name = "MZ and/or PE"sv;
			break;

		case executable::type::dalvik:
			name = "Dalvik"sv;
			break;
		}

		return fmt::formatter<std::string_view>::format(name, context);
	}
};
