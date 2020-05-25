// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include <git2/global.h>

namespace git
{

struct initializer
{
	initializer()
	{
		git_libgit2_init();
	}

	~initializer()
	{
		git_libgit2_shutdown();
	}

	initializer(const initializer &) = delete;
	initializer &operator=(const initializer &) = delete;
	initializer(initializer &&) = delete;
	initializer &operator=(initializer &&) = delete;
};

} // namespace git
