// SPDX-License-Identifier: GPL-3.0-or-later
#include "checks/g5.hpp"
#include "checks/checks.hpp" // for checks::level_t
#include "checks/stubs.hpp"	 // for checks::stubs::done_in_compiler_and_clang_tidy

// GNU warnings/clang-tidy cover this
void checks::g5::do_check(checks::level_t, managers::resources_manager &)
{
	checks::stubs::done_in_compiler_and_clang_tidy("g5");
}
