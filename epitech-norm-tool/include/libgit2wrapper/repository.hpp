// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include "libgit2wrapper/errors.hpp"
#include <git2/repository.h>
#include <memory>
#include <string>

namespace git
{

class repository
{
	struct destroy_git_repository
	{
		void operator()(git_repository *repo) const
		{
			git_repository_free(repo);
		}
	};

	std::unique_ptr<git_repository, destroy_git_repository> libgit2_handle;

public:
	repository(const char *directory)
	{
		git_repository *opened_repository;
		if (git_repository_open_ext(&opened_repository, directory, 0, nullptr))
			git::throw_git_error();

		this->libgit2_handle.reset(opened_repository);
	}

	repository(const std::string &directory)
		: repository(directory.c_str())
	{
	}

	git_repository *get_libgit2_handle() const
	{
		return this->libgit2_handle.get();
	}

	std::string workdir() const
	{
		return git_repository_workdir(this->libgit2_handle.get());
	}
};

} // namespace git
