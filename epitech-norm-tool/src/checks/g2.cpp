// SPDX-License-Identifier: GPL-3.0-or-later
#include "checks/g2.hpp"
#include "checks/checks.hpp" // for checks::level_5
#include "checks/stubs.hpp"	 // checks::stubs::done_in_clang_format

// clang-format covers this, sadly it doesn't let us check if there is *less* than 1 line seperating, but eh ¯\_(ツ)_/¯ (might do very basic check later)
void checks::g2::do_check(checks::level_t, managers::resources_manager &)
{
	checks::stubs::done_in_clang_format("g2");
}
