#pragma once
#include <string>
#include <string_view>

namespace basename_wrappers
{

std::string base_name(const std::string& path);
std::string directory_base_name(const std::string& path);

}
