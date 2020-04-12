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
#include <filesystem>


#include <boost/locale.hpp>
#include <boost/locale/boundary.hpp>
#include <boost/locale/conversion.hpp>
#include <fstream>
#include <map>
#include <thread>

///------------------------------------------------------------------------------------------------------------------///
/// CONFIG FUNCTIONS

std::string parseConf(std::string line) {
    std::string res = "";
    bool flag = false;
    for (auto c = line.begin(); c != line.end(); c++) {
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
    std::tuple<std::string, std::string, std::string, int> res = std::make_tuple("///", "///", "///", -1);
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
        } catch (std::exception &) {
            std::cout << "\nstoi Error\n\n";
            std::get<3>(res) = 1;
        }
    }
    return res;
}


bool validateConfigs(std::tuple<std::string, std::string, std::string, int> configs) {
    return (std::get<0>(configs) != "///") && (std::get<1>(configs) != "///") && (std::get<2>(configs) != "///") &&
           (std::get<3>(configs) != -1);
}

///------------------------------------------------------------------------------------------------------------------///
/// FILE READING FUNCTIONS

auto inline read_file_into_memory(const std::string &file_name) {

    std::ifstream raw_file(file_name, std::ios::binary);
    auto buffer = static_cast<std::ostringstream &>(
            std::ostringstream{} << raw_file.rdbuf()).str();

    return buffer;

}


template<typename CharT, typename Traits,
        typename Allocator = std::allocator<CharT>>

auto read_txt_into_memory(std::basic_ifstream<CharT, Traits> &in, Allocator alloc = {}) {
    using std::begin;
    using std::end;

    auto const chunk_size = std::size_t{BUFSIZ};

    auto container = std::vector<CharT, Allocator>(
            std::move(alloc));

    auto chunk = std::array<char, chunk_size>{};

    while (
            in.read(chunk.data(), chunk.size()) ||
            in.gcount())
        container.insert(end(container),
                         begin(chunk),
                         begin(chunk) + in.gcount());

    return container;
}

std::string file_as_str(std::vector<char> file_as_chars) {
    std::string data = "";

    for (auto c: file_as_chars) {
        if (c == '\n') {
            data += ' ';
        } else {
            data += c;
        }
    }

    return data;
}


///------------------------------------------------------------------------------------------------------------------///

auto read_txt(const std::string &file_name) {
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

auto slice_text(std::string &text) {

    boost::locale::generator gen;
    static auto loc = gen("en_US.UTF-8");

    text = boost::locale::normalize(text, boost::locale::norm_nfd, loc);
    text = boost::locale::fold_case(text, loc);

    boost::locale::boundary::ssegment_index words(boost::locale::boundary::word, text.begin(), text.end(), loc);
    words.rule(boost::locale::boundary::word_any);

    std::vector<std::string> vec_words;

    std::string current_string;
    for (auto it = words.begin(), e = words.end(); it != e; ++it) {
        vec_words.emplace_back((*it));
    }

    return vec_words;
}


/*
std::vector<std::string> split_equally(const std::string &str, int n) {
    std::vector<std::string> res;

    int startIdx = 0;
    for (int i = 0; i < n; i++) {
        if (i == n - 1) {
            res.push_back(str.substr(startIdx));
        } else {
            int increment = 0;
            int sliceLen = str.length() / n;
            while (str[startIdx + sliceLen + increment] != ' ') {
                increment++;
            }
            increment++;
            res.push_back(str.substr(startIdx, sliceLen + increment));
            startIdx = startIdx + sliceLen + increment;
        }
    }

    return res;
}
*/


void count_words(std::vector<std::string> &words_list, std::map<std::string, int> &dictionary, int start, int end) {

    std::string word;
    for (int i=start; i < end; i++) {
        word = words_list[i];
        dictionary[word] += 1;
    }
}

void trigger_function(int threadN, const std::string &input, const std::string &out_n, const std::string &out_a) {
    //one thread function

    //TODO many threads
    //TODO file output not console
    std::ofstream bya(out_a);
    std::ofstream byn(out_n);

    std::ifstream inp(input); // your filepath here

    auto data = read_txt_into_memory(inp);
    std::string tex = file_as_str(data);

    auto txts = slice_text(tex);
    std::map<std::string, int> dick;

    count_words(txts, dick, 0, txts.size());

    //TODO WRITE INTO FILES!
    for (auto x: dick) {
        std::cout << x.first << " " << x.second << "\n";
    }
}

void mergeMaps(std::vector<std::map<std::string, int>> &allMaps) {
    for (int i = 0; i < allMaps.size(); i++) {
        for (const auto &x: allMaps[i]) {
            allMaps[0][x.first] += x.second;
        }
    }
}


void parallel(int threadN, const std::string &input, const std::string &out_n, const std::string &out_a) {

    ///vector of maps
    std::vector<std::map<std::string, int>> allMaps;
    for (int i = 0; i < threadN; i++) {
        std::map<std::string, int> x;
        allMaps.push_back(x);
    }

    ///split words
    std::ofstream bya(out_a);
    std::ofstream byn(out_n);
    std::ifstream inp(input);
    auto data = read_txt_into_memory(inp);
    std::string tex = file_as_str(data);

    ///pass slices to threads
    //std::vector<std::string> slices = split_equally(tex, threadN);
    std::vector<std::thread> threads;

    std::vector<std::string> sss = slice_text(tex);
    int start;
    int end;
    for (int j = 0; j < threadN-1; j++) {
        start = j*sss.size()/threadN;
        end = ((j+1)*sss.size()/threadN) - 1;
        threads.emplace_back(count_words, std::ref(sss), std::ref(allMaps[j]), start, end);
    }

    threads.emplace_back(count_words, std::ref(sss), std::ref(allMaps[allMaps.size()-1]), end, sss.size());

    ///join
    for (auto & thread : threads) {
        thread.join();
    }

    ///merge maps
    mergeMaps(allMaps);

    for (const auto& x: allMaps[0]) {
        std::cout << x.first << " ---> " << x.second << "\n";
    }
}


///------------------------------------------------------------------------------------------------------------------///
/// MAIN PROGRAM

int main(int argc, char *argv[]) {

    std::string conf = "NONE";
    if (argc > 2) {
        std::cout << "Too many arguments. There should be 2 cmd arguments.\n";
        return 1;
    } else if (argc < 2) {
        if (std::filesystem::exists(std::filesystem::path("config.dat"))) {
            conf = "config.dat";
        }
    } else {
        conf = argv[1];
    }
    std::tuple<std::string, std::string, std::string, int> configs = configurate(conf);
    if (!validateConfigs(configs)) {
        std::cout << conf << "   <---- config file\n";
        std::cout << "Config error\n";
        return 1;
    }
    std::cout << std::get<0>(configs) << std::endl;
    std::cout << std::get<1>(configs) << std::endl;
    std::cout << std::get<2>(configs) << std::endl;
    std::cout << std::get<3>(configs) << std::endl;


    parallel(std::get<3>(configs), std::get<0>(configs), std::get<2>(configs), std::get<1>(configs));


    std::cout << "DONE";
    return 0;
}

///------------------------------------------------------------------------------------------------------------------///