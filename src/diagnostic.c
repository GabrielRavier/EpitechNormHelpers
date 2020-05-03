#include "diagnostic.h"
#include <string.h>
#include <stdbool.h>

const char *g_program_name = "";

static bool is_directory_seperator(char c)
{
	return c == '/';
}

void set_program_name(const char *argv0)
{
	const char *ptr = argv0 + strlen(argv0);
	while (ptr != argv0 && !is_directory_seperator(ptr[-1]))
		--ptr;

	g_program_name = ptr;
}
