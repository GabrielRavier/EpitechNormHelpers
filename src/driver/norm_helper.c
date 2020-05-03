// SPDX-License-Identifier: GPL-3.0-or-later
#include "driver/norm_helper.h"
#include <string.h>
#include <stdbool.h>

static bool is_directory_seperator(char c)
{
	return c == '/';
}

int driver_norm_helper_main(struct driver_norm_helper *const norm_helper, int argc, char *argv[])
{
	driver_norm_helper_set_program_name(norm_helper, argv[0]);
	return 0;
}

int driver_norm_helper_set_program_name(struct driver_norm_helper *const norm_helper, const char *argv0)
{
	const char *ptr = argv0 + strlen(argv0);
	while (ptr != argv0 && !is_directory_seperator(ptr[-1]))
		--ptr;
	return 0;
}
