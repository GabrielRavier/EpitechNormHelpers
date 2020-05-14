// SPDX-License-Identifier: GPL-3.0-or-later
#include "checks/checks.hpp"
#include "checks/o2.hpp"
#include "libgit2wrapper/global.hpp"
#include "libgit2wrapper/index.hpp"
#include "diagnostic.hpp"
#include "basename.hpp"
#include <boost/regex.hpp>

// Level 1 doesn't allow files with these extensions : *.C, *.H, *.CPP, *.HPP, *.hh, *.cc, *.i, *.f, *.F, *.FOR, *.FPP, *.FTN, *.m, *.M, *.d
static void do_level1(const git::index::file_list& filenames)
{
	for (std::string_view filename : filenames)
	{
		static const boost::regex basename_regex{R"delimiter((?:.*\.(?:[CHifFdmM]|CPP|HPP|hh|cc|FOR|FPP|FTN)))delimiter"};
		const std::string basename = basename_wrappers::base_name(filename);

		boost::smatch match;
		if (boost::regex_match(basename, match, basename_regex))
			diagnostic::warn(fmt::format("o2: '{}' matched level 1", filename));
	}
}

// Level 2 doesn't allow files with these extensions : *.cpp, *.cp, *.cxx, *.c++, *.hpp, *.hp, *.hx, *.h++, *.tcc, *.icc, *.ii, *.mi, *.mm, *.mii, *.di, *.dd, *.ads, *.adb, *.for, *.ftn, *.fpp, *.FPP, *.FTN, *.f90, *.f95, *.f03, *.f08, *.F90, *.F95, *.F03, *.F08, *.go, *.brig
static void do_level2(const git::index::file_list& filenames)
{
	for (std::string_view filename : filenames)
	{
		static const boost::regex basename_regex{R"delimiter((?:.*\.(?:[ch](?:pp?|xx|\+\+)|[ti]cc|ii|mii?|mm|d[id]|ad[sb]|f(?:or|tn|pp)|[fF](?:90|95|03|08)|go|brig)))delimiter"};
		const std::string basename = basename_wrappers::base_name(filename);

		boost::smatch match;
		if (boost::regex_match(basename, match, basename_regex))
			diagnostic::warn(fmt::format("o2: '{}' matched level 2", filename));
	}
}

// Level 3 doesn't allow files with these extensions : *.asm, *.s, *.S, *.sx
static void do_level3(const git::index::file_list& filenames)
{
	for (std::string_view filename : filenames)
	{
		static const boost::regex basename_regex{R"delimiter((?:.*\.(?:asm|[sS]|sx)))delimiter"};
		const std::string basename = basename_wrappers::base_name(filename);

		boost::smatch match;
		if (boost::regex_match(basename, match, basename_regex))
			diagnostic::warn(fmt::format("o2: '{}' matched level 3", filename));
	}
}

// Level 4 only allows : Makefile, GNUmakefile, CMakeLists.txt, configure.ac, configure, Makefile.in, .gitignore, .gitconfig, README*, COPYING, *.c, *.h and *.cmake files, files in LICENSES, doc, documentation, inc, include, ext, external, src and source directories
static void do_level4(const git::index::file_list& filenames)
{
	for (std::string_view filename : filenames)
	{
		static const boost::regex whitelisted_directories_regex{R"delimiter((?:^|\/)(?:src|source|doc|documentation|inc|include|ext|external|LICENSES)\/)delimiter"};

		boost::match_results<std::string_view::const_iterator> sv_match;
		if (!boost::regex_search(filename.begin(), filename.end(), sv_match, whitelisted_directories_regex))
		{
			static const boost::regex basename_regex{R"delimiter((?:GNUm|M)akefile|CMakeLists.txt|configure(?:.ac)?|Makefile.in|\.git(?:ignore|config|modules)|README.*|COPYING|.*\.(c|h|cmake))delimiter"};
			const std::string basename = basename_wrappers::base_name(filename);

			boost::smatch match;
			if (!boost::regex_match(basename, match, basename_regex))
				diagnostic::warn(fmt::format("o2: '{}' matched level 4", filename));
		}
	}
}

// Level 5 is the same as level 4, except it doesn't allow anything but .c and .h files in the src and source directories, and anything but .h files in the inc and include directories (i.e. if a path includes an 'include' or 'inc' directory, it will require the files in it to be .h files
static void do_level5(const git::index::file_list& filenames)
{
	for (std::string_view filename : filenames)
	{
		static const boost::regex inc_include_directories_regex{R"delimiter((?:^|\/)(?:inc|include)\/)delimiter"};
		static const boost::regex src_source_directories_regex{R"delimiter((?:^|\/)(?:src|source)\/)delimiter"};
		const std::string basename = basename_wrappers::base_name(filename);

		boost::match_results<std::string_view::const_iterator> sv_match;
		boost::regex file_regex;
		if (boost::regex_search(filename.begin(), filename.end(), sv_match, inc_include_directories_regex))
		{
			static const boost::regex header_regex{R"delimiter(.*\.h)delimiter"};
			file_regex = header_regex;
		}
		else if (boost::regex_search(filename.begin(), filename.end(), sv_match, src_source_directories_regex))
		{
			static const boost::regex source_regex{R"delimiter(.*\.(?:c|h))delimiter"};
			file_regex = source_regex;
		}
		else
		{
			continue;
		}

		boost::smatch match;
		if (!boost::regex_match(basename, match, file_regex))
			diagnostic::warn(fmt::format("o2: '{}' matched level 5", filename));
	}
}

void checks::o2::do_check(checks::level_t level, managers::resources_manager& check_resource_manager)
{
	git::index::file_list filenames = check_resource_manager.cwd_git.request_index().list_files();

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
