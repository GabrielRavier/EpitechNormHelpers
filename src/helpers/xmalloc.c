// SPDX-License-Identifier: GPL-3.0-or-later
#include "helpers/xmalloc.h"
#include "diagnostic.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static void xmalloc_failed(size_t size)
{
	fprintf(stderr, "\n%s%sOut of memory allocating %zu bytes\n", g_program_name, *g_program_name ? ": " : "", size);
	exit(EXIT_FAILURE);
}

void *xmalloc(size_t size)
{
	void *new_memory;

	if (size == 0)
		size = 1;

	new_memory = malloc(size);
	if (new_memory == NULL)
		xmalloc_failed(size);

	return new_memory;
}

void *xcalloc(size_t num_elements, size_t element_size)
{
	void *new_memory;
	if (num_elements == 0 || element_size == 0)
	{
		num_elements = 1;
		element_size = 1;
	}

	new_memory = calloc(num_elements, element_size);
	if (new_memory == NULL)
		xmalloc_failed(num_elements * element_size);

	return new_memory;
}

void *xrealloc(void *old_memory, size_t size)
{
	void *new_memory;

	if (size == 0)
		size = 1;

	if (!old_memory)
		new_memory = malloc(size);
	else
		new_memory = realloc(old_memory, size);

	if (new_memory == NULL)
		xmalloc_failed(size);

	return new_memory;
}
