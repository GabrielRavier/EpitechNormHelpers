#include "checks/o3.hpp"

static void do_level1()
{

}

void checks::o3(int level)
{
	if (level >= 1)
		do_level1();
}
