// SPDX-License-Identifier: GPL-3.0-or-later
#include "checks/f5.hpp"
#include "checks/stubs.hpp"			  // for checks::stubs::done_in_compiler

void checks::f5::do_check(checks::level_t, managers::resources_manager &)
{
	checks::stubs::done_in_compiler("f5");
}
