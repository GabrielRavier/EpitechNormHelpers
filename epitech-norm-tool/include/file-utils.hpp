// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include <cstddef>
#include <filesystem>
#include <optional>
#include <string>
#include <variant>

namespace file_utils
{

enum class error
{
	io_error,
	too_small,
};

std::optional<std::string> attempt_file_to_string(const std::filesystem::path &filename);
std::variant<std::string, error> attempt_file_to_string_with_size(const std::filesystem::path &filename, std::size_t size);

} // namespace file_utils
