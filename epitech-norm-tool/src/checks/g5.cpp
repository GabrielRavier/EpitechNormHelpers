// SPDX-License-Identifier: GPL-3.0-or-later
#include "checks/g5.hpp"
#include "checks/checks.hpp" // for checks::level_t
#include "diagnostic.hpp"	 // for diagnostic::error

// GNU warnings/clang-tidy cover this
void checks::g5::do_check(checks::level_t level, managers::resources_manager &)
{
	if (level >= 1)
		diagnostic::error("g5: this check is not implemented in the tool, you may want to look at gnu-warnings.txt or clang-tidy.txt for notes on how to do this check directly with your compiler or clang-tidy");
}
