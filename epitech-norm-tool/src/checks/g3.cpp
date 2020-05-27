// SPDX-License-Identifier: GPL-3.0-or-later
#include "checks/g3.hpp"
#include "checks/checks.hpp"	// for checks::level_t
#include "diagnostic.hpp"	// for diagnostic::error

// clang-tidy covers this
void checks::g3::do_check(checks::level_t level, managers::resources_manager &)
{
	if (level >= 1)
		diagnostic::error("g3: this check is not implemented in the tool, you may want to look at clang-tidy.txt for notes on how to do this check with clang-tidy");
}
