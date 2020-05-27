// SPDX-License-Identifier: GPL-3.0-or-later
#include "managers.hpp"
#include "diagnostic.hpp"					// for diagnostic::warn
#include "libgit2wrapper/global.hpp"		// for git::initializer
#include "libgit2wrapper/index.hpp"			// for git::index
#include "libgit2wrapper/repository.hpp"	// for git::repository
#include <cppast/cpp_entity_index.hpp>		// for cppast::cpp_entity_index
#include <cppast/detail/intrusive_list.hpp> // for cppast::detail::iteratable_intrusive_list
#include <cppast/libclang_parser.hpp>		// for cppast::libclang_parser, cppast::parse_database, etc.
#include <cppast/parser.hpp>				// for cppast::simple_file_parser
#include <filesystem>						// for std::filesystem::path
#include <optional>							// for std::optional
#include <type_safe/reference.hpp>			// for type_safe::ref

void managers::cwd_git_manager::request_git_initialization()
{
	if (!this->initializer.has_value())
		this->initializer.emplace();
}

const git::repository &managers::cwd_git_manager::request_repo()
{
	if (!this->repository.has_value())
	{
		this->request_git_initialization();
		this->repository.emplace(".");
	}

	return *this->repository;
}

const git::index &managers::cwd_git_manager::request_index()
{
	if (!this->index.has_value())
		this->index.emplace(this->request_repo());
	;

	return *this->index;
}

const git::index::file_list &managers::cwd_git_manager::request_file_list()
{
	if (!this->file_list.has_value())
		this->file_list.emplace(this->request_index().list_files());

	return *this->file_list;
}

managers::cppast_manager::cppast_manager(const std::filesystem::path &compile_options_directory_parameter)
	: compile_options_directory(compile_options_directory_parameter)
{
}

const cppast::cpp_entity_index &managers::cppast_manager::request_entity_index()
{
	if (!this->entity_index.has_value())
		this->entity_index.emplace();

	return *this->entity_index;
}

const cppast::libclang_compilation_database &managers::cppast_manager::request_compilation_database()
{
	if (!this->compilation_database.has_value())
		this->compilation_database.emplace(this->compile_options_directory);

	return *this->compilation_database;
}

cppast::detail::iteratable_intrusive_list<cppast::cpp_file> managers::cppast_manager::request_parsed_files_list()
{
	if (!this->parsed_files.has_value())
	{
		this->parsed_files.emplace(type_safe::ref(this->request_entity_index()));
		try
		{
			cppast::parse_database(*this->parsed_files, this->request_compilation_database());
		}
		catch (cppast::libclang_error &libclang_error)
		{
			this->parsed_files.reset();
			throw;
		}

		if (this->parsed_files->error())
			diagnostic::warn("a parse error occured, this may cause problems");
	}

	return this->parsed_files->files();
}
