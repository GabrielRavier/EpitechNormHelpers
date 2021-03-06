// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include <fmt/format.h>
#include <git2/errors.h>
#include <stdexcept>

namespace git
{

/**
 * @brief This should be called immediatly following a failed libgit2 call (before any other libgit2 calls)
 */
[[noreturn]] inline void throw_git_error()
{
	const git_error *last_git_error = git_error_last();
	throw std::runtime_error(fmt::format("git error {}: {}", last_git_error->klass, last_git_error->message));
}

} // namespace git
