#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>
#include <map>
#include <archive.h>
#include <archive_entry.h>

auto inline read_file_into_memory(const std::string &file_name) {

	std::ifstream raw_file(file_name, std::ios::binary);
	auto buffer = static_cast<std::ostringstream&>(
			std::ostringstream{} << raw_file.rdbuf()).str();
	std::cout << buffer << std::endl;
	return buffer;

}
int main() {

	std::string file = "file.zip";
	auto buf = read_file_into_memory(file);
	struct archive *a = archive_read_new();
	archive_read_support_compression_gzip(a);
	archive_read_support_format_tar(a);
	r = archive_read_open_memory(a, buff, sizeof(buff));
	std::cout << buf << std::endl;

//	std::map<std::string, int> new_map;
//	new_map["key"] = 12;
//
//	std::cout << new_map["key"] << std::endl;
//	return 0;
}