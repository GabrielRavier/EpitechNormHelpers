// SPDX-License-Identifier: GPL-3.0-or-later
#include "checks/f3.hpp"
#include "checks/checks.hpp" // for checks::level_t
#include "checks/stubs.hpp"	 // for checks::stubs::done_in_clang_format

// clang-format covers this
void checks::f3::do_check(checks::level_t, managers::resources_manager &)
{
	checks::stubs::done_in_clang_format("f3");
}
