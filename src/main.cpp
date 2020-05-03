// SPDX-License-Identifier: GPL-3.0-or-later
#include <cxxopts.hpp>
#include <fmt/format.h>
#include <exception>
#include <iostream>

static void program(const cxxopts::ParseResult& options)
{

}

static auto parseArguments(int argc, char *argv[])
{
	cxxopts::Options options("epitech-norm-helper", "- Helper for respecting Epitech norms");

	options.add_options()
			("check-organization-1", "Files organization - Contents of the delivery folder", cxxopts::value<unsigned>());
			("h,help", "Print usage");

	options.parse_positional({"directory"});
	options.show_positional_help();
	options.positional_help("[DIRECTORY]...");

	auto parsedOptions = options.parse(argc, argv);

	if (parsedOptions.count("help"))
	{
		std::cout << options.help();
		exit(EXIT_SUCCESS);
	}

	return parsedOptions;
}

int main(int argc, char *argv[])
{
	try
	{
		program(parseArguments(argc, argv));
		return EXIT_SUCCESS;
	}
	catch (const cxxopts::OptionParseException& optionParseException)
	{
		std::cerr << fmt::format("Failed to parse options: {}\n", optionParseException.what());
	}
	catch (const std::exception& exception)
	{
		std::cerr << fmt::format("Exception thrown: {}\n", exception.what());
	}
	catch (...)
	{
		std::cerr << "Exception thrown: Something not inherited from std::exception\n";
	}

	return EXIT_FAILURE;
}
