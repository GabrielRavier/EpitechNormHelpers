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

std::variant<std::string, file_utils::error> file_utils::attempt_file_to_string_with_size(const std::filesystem::path& filename, std::size_t size)
{
	std::ifstream file{std::string{filename}};

	std::error_code i_dont_give_a_fuck_about_the_error_code_also_why_cant_you_extend_the_lifetime_of_a_temporary_when_binding_to_a_non_const_reference;	// Errors are checked for using `file_size` so we don't care about this
	auto file_size = std::filesystem::file_size(filename, i_dont_give_a_fuck_about_the_error_code_also_why_cant_you_extend_the_lifetime_of_a_temporary_when_binding_to_a_non_const_reference);

	if (file_size == static_cast<std::uintmax_t>(-1))
		return file_utils::error::io_error;

	if (file_size < size)
		return file_utils::error::too_small;

	auto stream_buffer = file.rdbuf();
	if (!stream_buffer || file.fail())
		return file_utils::error::io_error;

	std::string result;
	result.reserve(size);

	std::copy_n(std::istreambuf_iterator{stream_buffer}, size, std::back_inserter(result));
	if (file.fail())
		return file_utils::error::io_error;

	return result;
}
