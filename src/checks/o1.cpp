#include "o1.hpp"

static void do_level1()
{

}

static void do_level2()
{

}

static void do_level3()
{

}

static void do_level4()
{

}

void checks::o1(int level)
{
	// Note: All these checks are assuming you're using git
	// Level 1 checks for .o, .gch, .a, .so, *~, .autosave and .d files in the git repo
	// Level 2 also tries to find valid ELF executables and [repo-name].* files (where '[repo-name]' is the name of the git repository) and warn about them
	// Level 3 only allows : Makefile, .gitignore, .gitconfig, README*, files in doc, documentation, src and source directories, .c and .h files
	// Level 4 doesn't allow anything but .c and .h files in the src and source directories
	if (level >= 1)
		do_level1();

	if (level >= 2)
		do_level2();

	if (level >= 3)
		do_level3();

	if (level >= 4)
		do_level4();
}
