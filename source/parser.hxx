#pragma once

#include <string>
#include <vector>

void convert_to_obj_file(std::string& src)
{
	size_t dot_pos = src.rfind('.');
	assert(dot_pos != std::string_npos);

	src = src.substr(0, dot_pos + 1);
	src += 'o';
}

void convert_to_obj_files(std::vector< std::string >& src_files)
{
	for(std::string& src: src_files) { convert_to_obj_file(src); }
}

std::string_view get_stem(std::string_view file)
{
	size_t dot_pos = file.rfind('.');
	assert(dot_pos != std::string_view::npos);

	return file.substr(0, dot_pos);
}

std::string_view get_extension(std::string_view file)
{
	size_t dot_pos = file.rfind('.');
	assert(dot_pos != std::string_view::npos);

	return file.substr(dot_pos + 1);
}

