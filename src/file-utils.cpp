// SPDX-License-Identifier: GPL-3.0-or-later
#include "file-utils.hpp"
#include <string>
#include <filesystem>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <optional>
#include <algorithm>
#include <iterator>

std::optional<std::string> file_utils::attempt_file_to_string(const std::filesystem::path& filename)
{
	std::ifstream file;
	file.open(std::string{filename});

	std::stringstream as_stream;
	as_stream << file.rdbuf();
	if (file.fail())
		return std::nullopt;

	return as_stream.str();
}

std::optional<std::string> file_utils::attempt_file_to_string_with_size(const std::filesystem::path& filename, std::size_t size)
{
	std::ifstream file{std::string{filename}};

	std::string result;
	result.reserve(size);

	auto stream_buffer = file.rdbuf();
	if (!stream_buffer || file.fail())
		return std::nullopt;

	std::copy_n(std::istreambuf_iterator{stream_buffer}, size, std::back_inserter(result));
	if (file.fail())
		return std::nullopt;

	return result;
}
