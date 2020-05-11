// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include "libgit2wrapper/repository.hpp"
#include <git2/index.h>
#include <vector>
#include <string>

namespace git
{

class index
{
	struct destroy_git_index
	{
		void operator()(git_index *repo) const
		{
			git_index_free(repo);
		}
	};

	std::unique_ptr<git_index, destroy_git_index> libgit2_handle;

public:
	using file_list = std::vector<std::string>;

	index(const git::repository& repository)
	{
		git_repository *libgit2_repository_handle = repository.get_libgit2_handle();
		git_index *opened_index;
		if (git_repository_index(&opened_index, libgit2_repository_handle))
			git::throw_git_error();

		this->libgit2_handle.reset(opened_index);
	}

	~index()
	{
		git_index_free(this->libgit2_handle.get());
	}

	size_t entrycount() const
	{
		return git_index_entrycount(this->libgit2_handle.get());
	}

	const git_index_entry *operator[](size_t i) const
	{
		return git_index_get_byindex(this->libgit2_handle.get(), i);
	}

	file_list list_files() const
	{
		size_t entry_count = this->entrycount();

		file_list result;
		for (size_t i = 0; i < entry_count; ++i)
			result.push_back((*this)[i]->path);

		return result;
	}
};

}
