#include "checks/checks.hpp"
#include <cppast/libclang_parser.hpp>
#include <fmt/format.h>
#include "diagnostic.hpp"

static void do_level1()
{
	const cppast::cpp_entity_index index{};
	cppast::libclang_compilation_database compilation_database{"build"};
	cppast::simple_file_parser<cppast::libclang_parser> parser{type_safe::ref(index)};
	try
	{
		cppast::parse_database(parser, compilation_database);
	}
	catch (cppast::libclang_error& libclang_error)
	{
		diagnostic::fatal_error(fmt::format("libclang: {}", libclang_error.what()));
	}

	if (parser.error())
		diagnostic::fatal_error(fmt::format("a parse error occured"));

	for (const auto& file : parser.files())
	{

	}
}

void checks::o3(int level)
{
	if (level >= 1)
		do_level1();
}
