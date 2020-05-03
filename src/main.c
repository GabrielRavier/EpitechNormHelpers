// SPDX-License-Identifier: GPL-3.0-or-later
#include "driver/norm_helper.h"

/**
 * @brief main - Just initializes a driver and starts it up
 * @return Returns to the OS what the driver returns
 */
int main(int argc, char *argv[])
{
	struct driver_norm_helper norm_helper;
	return driver_norm_helper_main(&norm_helper, argc, argv);
}
