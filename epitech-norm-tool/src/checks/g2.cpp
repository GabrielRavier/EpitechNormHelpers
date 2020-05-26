// SPDX-License-Identifier: GPL-3.0-or-later
#include "checks/g2.hpp"
#include "checks/checks.hpp"
#include "diagnostic.hpp"
#include <cppast/cpp_entity_kind.hpp>
#include <cppast/libclang_parser.hpp>
#include <cppast/visitor.hpp>
#include <fmt/format.h>

// clang-format covers this, sadly it doesn't let us check if there is *less* than 1 line seperating, but eh ¯\_(ツ)_/¯ (might do basic check later)
void checks::g2::do_check(checks::level_t level, managers::resources_manager &)
{
	if (level >= 1)
		diagnostic::error("g2: this check is not implemented in the tool, you may want to look at the provided .clang-format file");
}
