// SPDX-License-Identifier: GPL-3.0-or-later
#include "checks/f1.hpp"
#include "checks/checks.hpp" // for checks::level_t
#include "diagnostic.hpp"	 // for diagnostic::error

// Unimplementable unless I can develop mind reading technology or do some ML bullshit so ¯\_(ツ)_/¯
void checks::f1::do_check(checks::level_t level, managers::resources_manager &)
{
	if (level >= 1)
		diagnostic::error("f1 check unimplemented and unimplementable");
}
