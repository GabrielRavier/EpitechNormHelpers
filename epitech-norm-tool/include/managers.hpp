// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include "libgit2wrapper/global.hpp"		// for git::initializer
#include "libgit2wrapper/index.hpp"			// for git::index
#include "libgit2wrapper/repository.hpp"	// for git::repository
#include <cppast/cpp_entity_index.hpp>		// for cppast::cpp_entity_index
#include <cppast/detail/intrusive_list.hpp> // for cppast::detail::iteratable_intrusive_list
#include <cppast/libclang_parser.hpp>		// for cppast::libclang_compilation_database, etc.
#include <cppast/parser.hpp>				// for cppast::parser.hpp
#include <filesystem>						// for std::filesystem::path
#include <optional>							// for std::optional
#include <vector>							// for std::vector
#include <string>							// for std::string
namespace cppast { class cpp_file; } // forward declare cppast::cpp_file in case none of the above actually define it

namespace managers
{

class cwd_git_manager
{
	std::optional<git::initializer> initializer;
	std::optional<git::repository> repository; // Destroyed before initializer
	std::optional<git::index> index;		   // Destroyed before repository and initializer
	std::optional<git::index::file_list> file_list;
	std::optional<git::index::file_list> source_file_list;

	void request_git_initialization();

public:
	const git::repository &request_repo();
	const git::index &request_index();
	const git::index::file_list &request_file_list();
	const git::index::file_list &request_c_cpp_source_file_list();
};

class cppast_manager
{
	std::optional<cppast::cpp_entity_index> entity_index;
	std::optional<cppast::libclang_compilation_database> compilation_database;
	std::optional<cppast::simple_file_parser<cppast::libclang_parser>> parsed_files; // Destroyed before index
	std::filesystem::path compile_options_directory;

	const cppast::cpp_entity_index &request_entity_index();
	const cppast::libclang_compilation_database &request_compilation_database();

public:
	cppast_manager(const std::filesystem::path &compile_options_directory_parameter);
	decltype(parsed_files->files()) request_parsed_files_list();
};

struct resources_manager
{
	cwd_git_manager cwd_git;
	cppast_manager cppast;
};

}; // namespace managers
