// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include <cppast/cpp_entity_index.hpp>	// for cppast::cpp_entity_index
#include <cppast/libclang_parser.hpp>	// for cppast::libclang_compilation_database
#include <cppast/parser.hpp>	// for cppast::parser.hpp
#include <filesystem>	// for std::filesystem::path
#include <optional>	// for std::optional
#include <cppast/detail/intrusive_list.hpp>	// for cppast::detail::iteratable_intrusive_list
#include "libgit2wrapper/global.hpp"	// for git::initializer
#include "libgit2wrapper/index.hpp"	// for git::index
#include "libgit2wrapper/repository.hpp"	// for git::repository

namespace managers
{

class cwd_git_manager
{
	std::optional<git::initializer> initializer;
	std::optional<git::repository> repository; // Destroyed before initializer
	std::optional<git::index> index;		   // Destroyed before repository and initializer
	std::optional<git::index::file_list> file_list;

	void request_git_initialization();

public:
	const git::repository &request_repo();
	const git::index &request_index();
	const git::index::file_list &request_file_list();
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
	cppast::detail::iteratable_intrusive_list<cppast::cpp_file> request_parsed_files_list();
};

struct resources_manager
{
	cwd_git_manager cwd_git;
	cppast_manager cppast;
};

}; // namespace managers
