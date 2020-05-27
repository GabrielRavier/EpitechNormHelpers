// SPDX-License-Identifier: GPL-3.0-or-later
#include "checks/g4.hpp"
#include "checks/checks.hpp" // for checks::level_t
#include "checks/stubs.hpp"	 // for checks::stubs::done_in_clang_tidy

// clang-tidy covers this
void checks::g4::do_check(checks::level_t, managers::resources_manager &)
{
	checks::stubs::done_in_clang_tidy("g4");
}
