#pragma once
#include "libgit2wrapper/repository.hpp"
#include <git2/index.h>

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
	index(const git::repository& repository)
	{
		git_repository *libgit2_repository_handle = repository.get_libgit2_handle();
		git_index *opened_index;
		if (git_repository_index(&opened_index, libgit2_repository_handle))
			git::throw_git_error();

		this->libgit2_handle.reset(opened_index);
	}

	size_t entrycount() const
	{
		return git_index_entrycount(this->libgit2_handle.get());
	}

	const git_index_entry *operator[](size_t i) const
	{
		return git_index_get_byindex(this->libgit2_handle.get(), i);
	}
};

}
