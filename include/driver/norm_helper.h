// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

struct driver_norm_helper
{

};

/**
 * @brief driver_norm_helper_main - Main component of the driver
 * @param argc - argc as passed to main
 * @param argv - argv as passed to main
 */
int driver_norm_helper_main(struct driver_norm_helper *const norm_helper, int argc, char *argv[]);

/**
 * @brief driver_norm_helper_set_program_name - Locates the final component of argv[0] after any leading path, and sets the program name accordingly
 * @param argv0 - Should be argv[0] as passed to main
 * @return
 */
int driver_norm_helper_set_program_name(struct driver_norm_helper *const norm_helper, const char *argv0);
