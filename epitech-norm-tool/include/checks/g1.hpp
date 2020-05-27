// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include "checks.hpp" // for checks::level_t, managers::resources_manager

namespace checks
{

namespace g1
{

void do_check(checks::level_t level, managers::resources_manager &check_resource_manager);

}

} // namespace checks
