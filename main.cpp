///------------------------------------------------------------------------------------------------------------------///
/// INCLUDES

#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>
#include <map>
#include <archive.h>
#include <archive_entry.h>
#include <string>
#include <tuple>
#include <vector>

///------------------------------------------------------------------------------------------------------------------///
/// CONFIG FUNCTIONS

std::string parseConf(std::string line) {
    std::string res = "";
    bool flag = false;
    for (auto c=line.begin(); c != line.end(); c++) {
        if (flag && *c != '"') {
            res += *c;
        }
        if (*c == '=') {
            flag = true;
        }
    }
    return res;
}


std::tuple<std::string, std::string, std::string, int> configurate(std::string filename) {
    std::ifstream conf(filename);
    std::string line;
    std::tuple<std::string, std::string, std::string, int> res = std::make_tuple("///","///","///",-1);
    if (conf.is_open()) {
        std::getline(conf, line);
        std::get<0>(res) = parseConf(line);

        std::getline(conf, line);
        std::get<1>(res) = parseConf(line);

        std::getline(conf, line);
        std::get<2>(res) = parseConf(line);

        std::getline(conf, line);
        try {
            std::get<3>(res) = std::stoi(parseConf(line));
        } catch(std::exception&) {
            std::cout << "\nstoi Error\n\n";
            std::get<3>(res) = 1;
        }
    }
    return res;
}


bool validateConfigs(std::tuple<std::string, std::string, std::string, int> configs) {
    return (std::get<0>(configs) != "///") && (std::get<1>(configs) != "///") && (std::get<2>(configs) != "///") && (std::get<3>(configs) != -1);
}

///------------------------------------------------------------------------------------------------------------------///
/// FILE READING FUNCTIONS

auto inline read_file_into_memory(const std::string &file_name) {

	std::ifstream raw_file(file_name, std::ios::binary);
	auto buffer = static_cast<std::ostringstream&>(
			std::ostringstream{} << raw_file.rdbuf()).str();
	std::cout << buffer << std::endl;
	return buffer;

}

///------------------------------------------------------------------------------------------------------------------///
/// MAIN PROGRAM

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

///------------------------------------------------------------------------------------------------------------------///