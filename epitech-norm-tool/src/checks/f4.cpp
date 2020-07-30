// SPDX-License-Identifier: GPL-3.0-or-later
#include "checks/f4.hpp"
#include "checks/stubs.hpp"			  // for checks::stubs::done_in_clang_tidy

void checks::f4::do_check(checks::level_t ,managers::resources_manager &)
{
	checks::stubs::done_in_clang_tidy("f4");
}
