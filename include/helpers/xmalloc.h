// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include <stddef.h>

/**
 * @brief Allocates memory without fail. If malloc fails, this will print a message to stderr and then call exit
 */
void *xmalloc(size_t size);

/**
 * @brief Allocates memory without fail, and sets it to zero. Functions like calloc, but will behave the same as xmalloc if reallocation failsexit
 */
void *xcalloc(size_t num_elements, size_t element_size);

/**
 * @brief Reallocates memory without fail. Functions like realloc, but will behave the same as xmalloc if reallocation fails
 */
void *xrealloc(void *old_memory, size_t size);
