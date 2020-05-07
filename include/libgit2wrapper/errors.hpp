#pragma once
#include <string_view>
#include <stdexcept>
#include <fmt/format.h>
#include <git2/errors.h>

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

}
