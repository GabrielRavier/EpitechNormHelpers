// SPDX-License-Identifier: GPL-3.0-or-later
#include "executable-type.hpp"
#include <cstddef> // for std::size_t
#include <fstream> // for std::fstream, std::istream::read
#include <string>  // for std::string

static bool is_valid_elf(const unsigned char *buffer, std::size_t length)
{
	if (length < 4)
		return false;

	return (buffer[0] == 0x7F) && (buffer[1] == 0x45) && (buffer[2] == 0x4C) && (buffer[3] == 0x46);
}

static bool is_valid_mz(const unsigned char *buffer, std::size_t length)
{
	if (length < 2)
		return false;

	return (buffer[0] == 0x4D) && (buffer[1] == 0x5A);
}

static bool is_valid_dalvik(const unsigned char *buffer, std::size_t length)
{
	if (length < 8)
		return false;

	return (buffer[0] == 0x64) && (buffer[1] == 0x65) && (buffer[2] == 0x78) && (buffer[3] == 0xA) && (buffer[4] == 0x30) && (buffer[5] == 0x33) && (buffer[6] == 0x35) && (buffer[7] == 0x0);
}

executable::type executable::get_type_from_stream(std::ifstream &file_stream)
{
	char buffer[8];
	file_stream.read(buffer, sizeof(buffer));
	size_t read_length = file_stream.gcount();
	unsigned char *unsigned_char_buffer = reinterpret_cast<unsigned char *>(buffer);

	if (is_valid_elf(unsigned_char_buffer, read_length))
		return executable::type::elf;

	if (is_valid_mz(unsigned_char_buffer, read_length))
		return executable::type::mz;

	if (is_valid_dalvik(unsigned_char_buffer, read_length))
		return executable::type::dalvik;

	return executable::type::none;
}

executable::type executable::get_type_from_file(std::filesystem::path filename)
{
	if (std::filesystem::is_directory(filename))
		return executable::type::none;

	std::ifstream file_stream;
	file_stream.exceptions(std::ifstream::badbit);
	file_stream.open(std::string{filename}, std::ifstream::binary);

	return executable::get_type_from_stream(file_stream);
}
