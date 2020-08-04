// SPDX-License-Identifier: GPL-3.0-or-later
#include "checks/f7.hpp"
#include "checks/stubs.hpp"	// for checks::done_in_compiler_and_clang_tidy

void checks::f7::do_check(checks::level_t, managers::resources_manager &)
{
	checks::stubs::done_in_compiler_and_clang_tidy("f7");
}
