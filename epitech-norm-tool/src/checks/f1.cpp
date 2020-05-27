// SPDX-License-Identifier: GPL-3.0-or-later
#include "checks/f1.hpp"
#include "checks/checks.hpp" // for checks::level_t
#include "checks/stubs.hpp"	 // for checks::stubs::unimplementable

// Unimplementable unless I can develop mind reading technology or do some ML bullshit so ¯\_(ツ)_/¯
void checks::f1::do_check(checks::level_t, managers::resources_manager &)
{
	checks::stubs::unimplementable("f1");
}
