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


#include <boost/locale.hpp>
#include <boost/locale/boundary.hpp>
#include <boost/locale/conversion.hpp>
#include <fstream>
#include <map>

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

	return buffer;

}

//--------------------------------------------------------------------------------------------------------------------//

auto read_txt( const std::string &file_name ){
    std::string content, word;
    std::ifstream file;

    file.open(file_name);
    if (!file) {
        std::cout << "Unable to open file";
        exit(1);
    }

    while (file >> word) {
        content += word + " ";
    }

    file.close();
    return content;
}

auto slice_text(std::string &text){

    boost::locale::generator gen;
    static auto loc = gen("en_US.UTF-8");

    text = boost::locale::normalize(text, boost::locale::norm_nfd, loc);
    text = boost::locale::fold_case(text, loc);

    boost::locale::boundary::ssegment_index words( boost::locale::boundary::word, text.begin(), text.end(),loc );
    words.rule(boost::locale::boundary::word_any);

    return words;
}

auto count_words( boost::locale::boundary::ssegment_index &words_list ){
    std::map<std::string, int> dictionary;

    for ( auto word: words_list ){
        dictionary[word] += 1;
    }
    return dictionary;
}

auto trigger_function(){
//    one thread function

    std::string tex = read_txt("/home/vlad/Desktop/2_year/ACS/lab_3/a/counting_words/file1.txt"); // your filepath here
    auto txts = slice_text(tex);
    auto dick = count_words(txts);

    for (auto x: dick){
        std::cout << " " << x.first << " " << x.second << "\n";
    }
}

///------------------------------------------------------------------------------------------------------------------///
/// MAIN PROGRAM

int main() {

    trigger_function();

//	std::string file = "/home/vlad/Desktop/2_year/ACS/lab_3/a/counting_words/file.zip";
//	auto buf = read_file_into_memory(file);
//	struct archive *a = archive_read_new();
//	archive_read_support_compression_gzip(a);
//	archive_read_support_format_tar(a);
//	auto r = archive_read_open_memory(a, buf, sizeof(buf));


//	std::cout << buf << std::endl;

//	std::map<std::string, int> new_map;
//	new_map["key"] = 12;
//
//	std::cout << new_map["key"] << std::endl;
	return 0;
}

///------------------------------------------------------------------------------------------------------------------///