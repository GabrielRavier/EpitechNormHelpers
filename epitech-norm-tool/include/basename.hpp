// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include <string>
#include <string_view>

namespace basename_wrappers
{

std::string base_name(std::string_view path);
std::string directory_base_name(std::string_view path);

} // namespace basename_wrappers
