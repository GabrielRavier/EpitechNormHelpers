// SPDX-License-Identifier: GPL-3.0-or-later
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

	initializer(const initializer&) = delete;
	initializer& operator=(const initializer&) = delete;
};

}
