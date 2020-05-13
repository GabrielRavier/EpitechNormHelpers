// SPDX-License-Identifier: GPL-3.0-or-later
#include "checks/checks.hpp"
#include "libgit2wrapper/global.hpp"
#include "libgit2wrapper/repository.hpp"
#include "libgit2wrapper/index.hpp"
#include "diagnostic.hpp"
#include "basename.hpp"
#include "executable-type.hpp"
#include "regex-utils.hpp"
#include <vector>
#include <string>
#include <unordered_set>
#include <fmt/format.h>
#include <boost/regex.hpp>

// Improvement idea for this : Remove regexes and match everything individually to give a better warning message

// Level 1 checks for *.o, *.elf, *.obj, *.gch, *.pch, *.a, *.lib, *.exe, *.out, *.app, *.so, *.so.*, *.dylib, *.dll, *~, #*#, .#*, Session.vim, Sessionx.vim, *.autosave, CMakeLists.txt.user, CMakeCache.txt, cmake_install.cmake, install_manifest.txt, compile_commands.json and *.d files in the git repo
static void do_level1(const git::index::file_list& filenames)
{
	for (std::string_view filename : filenames)
	{
		static const boost::regex basename_regex{R"delimiter((?:(?:(?:.*(?:\.(?:[oad]|elf|obj|[gp]ch|lib|exe|out|app|so(\..*)?|dylib|dll|autosave)|~))|#.*#|\.#.*|Sessionx?\.vim|CMakeLists\.txt\.user|(?:CMakeCache|install_manifest)\.txt|cmake_install\.cmake|compile_commands\.json)))delimiter"};
		const std::string basename = basename_wrappers::base_name(filename);

		boost::smatch match;
		if (boost::regex_match(basename, match, basename_regex))
			diagnostic::warn(fmt::format("o1: '{}' matched level 1", filename));
	}
}

// Level 2 also tries to find valid ELF/PE/Dalvik executables and [repo-name].* files (where '[repo-name]' is the base name of the root directory of the git repository) and warn about them
static void do_level2(const git::index::file_list& filenames, const std::string& git_root_repository_name)
{
	for (std::string_view filename : filenames)
	{
		static const boost::regex basename_regex{fmt::format(R"delimiter((?:{}.*))delimiter", regex_utils::escape_string_for_insertion_in_regex(git_root_repository_name))};
		const std::string basename = basename_wrappers::base_name(filename);

		boost::smatch match;
		if (boost::regex_match(basename, match, basename_regex))
			diagnostic::warn(fmt::format("o1: '{}' matched level 2", filename));

		executable::type executable_type = executable::get_type_from_file(filename);
		if (executable_type != executable::type::none)
			diagnostic::warn(fmt::format("o1: '{}' matched level 2 executable check, being a valid {} executable", filename, executable_type));
	}
}

// Level 3 also checks for *.i*86, *.x86_64, *.hex, *.slo, *.lo, *.ko, *.lai, *.la, *.mod, *.smod, *.ilk, *.map, *dSYM/, *.su, *.idb, *.pdb, *.mod*, *.cmd, .tmp_versions/, modules.order, Module.symvers, Mkfile.old, dkms.conf, .dir-locals.el, [._]*.s[a-v][a-z], [._]*.sw[a-p], [._]s[a-rt-v][a-z], [._]ss[a-gi-z], [._]sw[a-p], [._]*.un~ and *.exp files in the git repo
static void do_level3(const git::index::file_list& filenames)
{
	std::unordered_set<std::string> matched_directories;	// Have a set of matched directories, so we can then just print them all in the end (duplicate matches will be removed since this is a set)
	for (std::string_view filename : filenames)
	{
		static const boost::regex basename_regex{R"delimiter((?:(?:(?:.*\.(?:i.*86|x86_64|hex|s?lo|ko|lai?|s?mod|ilk|map|su|[ip]db|mod.*|cmd|exp|s[a-v][a-z]|sw[a-p]|un~))|(?:modules\.order|Module\.symvers|Mkfile\.old|dkms\.conf|\.dir-locals\.el)|(?:[._](?:s[a-rt-v][a-z]|ss[a-gi-z]|sw[a-p])))))delimiter"};
		const std::string basename = basename_wrappers::base_name(filename);

		boost::smatch match;
		if (boost::regex_match(basename, match, basename_regex))
			diagnostic::warn(fmt::format("o1: '{}' matched level 3", filename));

		static const boost::regex directory_regex{R"delimiter(.*dSYM\/|(?:.*\/|^)\.tmp_versions\/)delimiter"};	// Made specifically to match the entire path up to the '/' after the directory name
		boost::match_results<std::string_view::const_iterator> sv_match;
		if (boost::regex_search(filename.begin(), filename.end(), sv_match, directory_regex))
			matched_directories.insert(sv_match[0].str());
	}

	for (std::string_view directory : matched_directories)
		diagnostic::warn(fmt::format("o1: '{}' directory matched level 3", directory));
}


void checks::o1(checks::level_t level)
{
	git::initializer libgit2_initializer;
	git::repository repository_in_cwd{"."};
	git::index::file_list filenames = git::index{repository_in_cwd}.list_files();

	// Note: All these checks are assuming you're using git
	if (level >= 1)
		do_level1(filenames);

	if (level >= 2)
	{
		std::string working_directory_basename = basename_wrappers::base_name(repository_in_cwd.workdir());
		do_level2(filenames, working_directory_basename);
	}

	if (level >= 3)
		do_level3(filenames);
}
