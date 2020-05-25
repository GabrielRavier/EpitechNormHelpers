// SPDX-License-Identifier: GPL-3.0-or-later
#include "checks/g2.hpp"
#include "checks/checks.hpp"
#include <cppast/libclang_parser.hpp>
#include <cppast/visitor.hpp>
#include <cppast/cpp_entity_kind.hpp>
#include <fmt/format.h>
#include "diagnostic.hpp"

void checks::g2::do_check(checks::level_t level, managers::resources_manager&)
{
	if (level >= 1)
		diagnostic::error("g2: this check is not implemented in the tool, you may want to look at the provided .clang-format file");
}
