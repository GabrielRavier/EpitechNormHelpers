// SPDX-License-Identifier: GPL-3.0-or-later
#include "checks/o1.hpp"
#include "libgit2wrapper/global.hpp"
#include "libgit2wrapper/repository.hpp"
#include "libgit2wrapper/index.hpp"
#include "diagnostic.hpp"
#include "basename.hpp"
#include "executable_type.hpp"
#include <vector>
#include <string>
#include <string_view>
#include <span>
#include <fstream>
#include <stdexcept>
#include <unordered_set>
#include <fmt/format.h>
#include <boost/regex.hpp>

// Improvement idea for this : Remove regexes and match everything individually to give a better warning message

using filenames_container = std::vector<std::string>;

static std::string escape_string_for_regex(const std::string& string)
{
	static const boost::regex escape_regex{R"delimiter([.^$|()\[\]{}*+?\\])delimiter"};
	constexpr const char *replace_regex{R"delimiter(\\&)delimiter"};
	return boost::regex_replace(string, escape_regex, replace_regex, boost::match_default | boost::format_sed);
}

static std::string get_git_root_directory_name()
{
	git::initializer initialize_libgit2;
	git::repository repository_in_cwd(".");

	return basename_wrappers::base_name(repository_in_cwd.workdir());
}

// Level 1 checks for *.o, *.elf, *.obj, *.gch, *.pch, *.a, *.lib, *.exe, *.out, *.app, *.so, *.so.*, *.dylib, *.dll, *~, #*#, .#*, Session.vim, Sessionx.vim, *.autosave, CMakeLists.txt.user, CMakeCache.txt, cmake_install.cmake, install_manifest.txt, compile_commands.json and *.d files in the git repo
static void do_level1(const filenames_container& filenames)
{
	for (const auto& filename : filenames)
	{
		static const boost::regex basename_regex{R"delimiter((?:((.*(\.(o|elf|obj|gch|pch|a|lib|exe|out|app|so|so\..*|dylib|dll|d|autosave)|~))|#.*#|\.#.*|Sessionx?\.vim|CMakeLists\.txt\.user|(CMakeCache|install_manifest)\.txt|cmake_install\.cmake|compile_commands\.json)))delimiter"};
		const std::string basename = basename_wrappers::base_name(filename);

		boost::smatch match;
		if (boost::regex_match(basename, match, basename_regex))
			diagnostic::warn(fmt::format("o1: '{}' matched level 1", filename), false);
	}
}

// Level 2 also tries to find valid ELF/PE/Dalvik executables and [repo-name].* files (where '[repo-name]' is the base name of the root directory of the git repository) and warn about them
static void do_level2(const filenames_container& filenames)
{
	std::string git_root_repository_name = get_git_root_directory_name();
	for (const auto& filename : filenames)
	{
		static const boost::regex basename_regex{fmt::format(R"delimiter((?:{}.*))delimiter", escape_string_for_regex(git_root_repository_name))};
		const std::string basename = basename_wrappers::base_name(filename);

		boost::smatch match;
		if (boost::regex_match(basename, match, basename_regex))
			diagnostic::warn(fmt::format("o1: '{}' matched level 2", filename), false);

		executable::type executable_type = executable::get_type_from_file(filename);
		if (executable_type != executable::type::none)
			diagnostic::warn(fmt::format("o1: '{}' matched level 2 executable check, being a valid {} executable", filename, executable_type), false);
	}
}

// Level 3 also checks for *.i*86, *.x86_64, *.hex, *.slo, *.lo, *.ko, *.lo, *.lai, *.la, *.mod, *.smod, *.ilk, *.map, *dSYM/, *.su, *.idb, *.pdb, *.mod*, *.cmd, .tmp_versions/, modules.order, Module.symvers, Mkfile.old, dkms.conf, .dir-locals.el, [._]*.s[a-v}[a-z], [._]*.sw[a-p], [._]s[a-rt-v][a-z], [._]ss[a-gi-z], [._]sw[a-p], [._]*.un~ and *.exp files in the git repo
static void do_level3(const filenames_container& filenames)
{
	std::unordered_set<std::string> matched_directories;	// Have a set of matched directories, so we can then just print them all in the end (duplicate matches will be removed since this is a set)
	for (const auto& filename : filenames)
	{
		static const boost::regex basename_regex{R"delimiter((?:((.*\.(i.*86|x86_64|hex|slo|lo|ko|lo|lai|la|mod|smod|ilk|map|su|idb|pdg|mod.*|cmd|exp))|(modules\.order|Module\.symvers|Mkfile\.old|dkms\.conf|\.dir-locals\.el)|([._](s[a-rt-v][a-z]|ss[a-gi-z]|sw[a-p]|.*\.(s[a-v}[a-z]|sw[a-p]|un~))))))delimiter"};
		const std::string basename = basename_wrappers::base_name(filename);

		boost::smatch match;
		if (boost::regex_match(basename, match, basename_regex))
			diagnostic::warn(fmt::format("o1: '{}' matched level 3", filename), false);

		static const boost::regex directory_regex{R"delimiter(.*dSYM\/|(?:.*\/|^)\.tmp_versions\/)delimiter"};	// Made specifically to match the entire path up to the '/' after the directory name
		if (boost::regex_search(filename, match, directory_regex))
			matched_directories.insert(match[0].str());
	}

	for (const auto& directory : matched_directories)
		diagnostic::warn(fmt::format("o1: '{}' directory matched level 3", directory), false);
}

// Level 4 only allows : Makefile, GNUmakefile, CMakeLists.txt, configure.ac, configure, Makefile.in, .gitignore, .gitconfig, README*, COPYING, *.c, *.h and *.cmake files, files in LICENSES, doc, documentation, inc, include, ext, external, src and source directories
static void do_level4(const filenames_container& filenames)
{
	for (const auto& filename : filenames)
	{
		static const boost::regex whitelisted_directories_regex{R"delimiter((?:^|\/)(?:src|source|doc|documentation|inc|include|ext|external|LICENSES)\/)delimiter"};

		boost::smatch match;
		if (!boost::regex_search(filename, match, whitelisted_directories_regex))
		{
			static const boost::regex basename_regex{R"delimiter((?:GNUm|M)akefile|CMakeLists.txt|configure(?:.ac)?|Makefile.in|\.git(?:ignore|config)|README.*|COPYING|.*\.(c|h|cmake))delimiter"};
			const std::string basename = basename_wrappers::base_name(filename);
			if (!boost::regex_match(basename, match, basename_regex))
				diagnostic::warn(fmt::format("o1: '{}' matched level 4", filename), false);
		}
	}
}

// Level 5 is the same as level 4, except it doesn't allow anything but .c and .h files in the src and source directories, and anything but .h files in the inc and include directories (i.e. if a path includes an 'include' or 'inc' directory, it will require the files in it to be .h files
static void do_level5(const filenames_container& filenames)
{
	for (const auto& filename : filenames)
	{
		static const boost::regex inc_include_directories_regex{R"delimiter((?:^|\/)(?:inc|include)\/)delimiter"};
		static const boost::regex src_source_directories_regex{R"delimiter((?:^|\/)(?:src|source)\/)delimiter"};
		const std::string basename = basename_wrappers::base_name(filename);

		boost::smatch match;
		boost::regex file_regex;
		if (boost::regex_search(filename, match, inc_include_directories_regex))
		{
			static const boost::regex header_regex{R"delimiter(.*\.h)delimiter"};
			file_regex = header_regex;
		}
		else if (boost::regex_search(filename, match, src_source_directories_regex))
		{
			static const boost::regex source_regex{R"delimiter(.*\.(?:c|h))delimiter"};
			file_regex = source_regex;
		}
		else
		{
			continue;
		}

		if (!boost::regex_match(basename, match, file_regex))
			diagnostic::warn(fmt::format("o1: '{}' matched level 5", filename), false);
	}
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
