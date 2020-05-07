// SPDX-License-Identifier: GPL-3.0-or-later
#include "checks/o1.hpp"
#include "libgit2wrapper/global.hpp"
#include "libgit2wrapper/repository.hpp"
#include "libgit2wrapper/index.hpp"
#include "compile-time-regular-expression/ctre.hpp"
#include "diagnostic.hpp"
#include <vector>
#include <string>
#include <string_view>
#include <fmt/format.h>

using filenames_container = std::vector<std::string>;

static std::string get_basename(std::string_view filename)
{
	size_t slash_pos = filename.find_last_of('/');
	if (slash_pos != std::string_view::npos)
		return {filename.begin() + slash_pos + 1, filename.end()};
	else
		return std::string{filename};
}

// Level 1 checks for *.o, *.elf, *.obj, *.gch, *.pch, *.a, *.lib, *.exe, *.out, *.app, *.so, *.so.*, *.dylib, *.dll, *~, #*#, .#*, Session.vim, Sessionx.vim, *.autosave, CMakeLists.txt.user, CMakeCache.txt, cmake_install.cmake, install_manifest.txt, compile_commands.json and *.d files in the git repo
static void do_level1(const filenames_container& filenames)
{
	for (const auto& filename : filenames)
	{
		static constexpr auto basename_regex = ctll::fixed_string(R"delimiter((.*\.o)|(.*\.elf)|(.*\.obj)|(.*\.gch)|(.*\.pch)|(.*\.a)|(.*\.lib)|(.*\.exe)|(.*\.out)|(.*\.app)|(.*\.so)|(.*\.so\..*)|(.*\.dylib)|(.*\.dll)|(.*~)|(#.*#)|(\.#.*)|(Session\.vim)|(Sessionx\.vim)|(.*\.autosave)|(CMakeLists\.txt\.user)|(CMakeCache\.txt)|(cmake_install\.cmake)|(install_manifest\.txt)|(compile_commands\.json)|(.*\.d))delimiter");
		std::string basename = get_basename(filename);

		if (ctre::match<basename_regex>(basename))
			diagnostic::warn(fmt::format("o1: {} matched level 1", filename), false);
	}
}

// Level 2 also tries to find valid ELF executables and [repo-name].* files (where '[repo-name]' is the name of the git repository) and warn about them
static void do_level2(const filenames_container& filenames)
{

}

// Level 3 also checks for *.i*86, *.x86_64, *.hex, *.slo, *.lo, *.ko, *.lo, *.lai, *.la, *.mod, *.smod, *.ilk, *.map, *dSYM/, *.su, *.idb, *.pdb, *.mod*, *.cmd, .tmp_versions/, modules.order, Module.symvers, Mkfile.old, dkms.conf, .dir-locals.el, [._]*.s[a-v}[a-z], [._]*.sw[a-p], [._]s[a-rt-v][a-z], [._]ss[a-gi-z], [._]sw[a-p], [._]*.un~ and *.exp files in the git repo
static void do_level3(const filenames_container& filenames)
{

}

// Level 4 only allows : Makefile, .gitignore, .gitconfig, README*, files in doc, documentation, src and source directories, .c and .h files
static void do_level4(const filenames_container& filenames)
{

}

// Level 5 is the same as level 5, except it doesn't allow anything but .c and .h files in the src and source directories
static void do_level5(const filenames_container& filenames)
{

}

static filenames_container list_files_in_git_repo(const char *repository_path)
{
	git::initializer initialize_libgit2;
	git::repository repository_in_cwd(repository_path);
	git::index repository_in_cwd_index(repository_in_cwd);
	size_t entry_count = repository_in_cwd_index.entrycount();

	filenames_container result;
	for (size_t i = 0; i < entry_count; ++i)
		result.push_back(repository_in_cwd_index[i]->path);

	return result;
}

void checks::o1(int level)
{
	filenames_container filenames = list_files_in_git_repo(".");

	// Note: All these checks are assuming you're using git
	if (level >= 1)
		do_level1(filenames);

	if (level >= 2)
		do_level2(filenames);

	if (level >= 3)
		do_level3(filenames);

	if (level >= 4)
		do_level4(filenames);

	if (level >= 5)
		do_level5(filenames);
}
