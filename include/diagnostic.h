#pragma once

extern const char *g_program_name;

/**
 * @brief Locates the final component of argv[0] after any leading path, and sets the program name accordingly
 */
void set_program_name(const char *argv0);
