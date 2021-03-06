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
#include <thread>

///------------------------------------------------------------------------------------------------------------------///
auto read_archive( const char *filename ){

    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

    struct archive *a;
    struct archive_entry *entry;

    a = archive_read_new();
    archive_read_support_filter_all(a);
    std::string content;

    archive_read_support_format_all(a);
    archive_read_open_filename(a, filename, 10240);

    while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {

        auto size = archive_entry_size(entry);
        content = std::string(size, 0);

        archive_read_data(a, &content[0], content.size());
        archive_read_data_skip(a);
    }
    archive_read_free(a);

    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    std::cout << "Loading: " << time_span.count() << "\n";

    return content;
}

auto file_definer( const std::string &filename ){
    return filename.substr(filename.find_last_of(".") + 1) == "txt";
}

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
           (std::get<3>(configs) > 0);
}

///------------------------------------------------------------------------------------------------------------------///
/// FILE READING FUNCTIONS

auto inline read_file_into_memory(const std::string &file_name) {

    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

    std::ifstream raw_file(file_name, std::ios::binary);
    auto buffer = static_cast<std::ostringstream &>(
            std::ostringstream{} << raw_file.rdbuf()).str();

    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    std::cout << "Loading: " << time_span.count() << "\n";

    return buffer;

}
///------------------------------------------------------------------------------------------------------------------///

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

void count_words(std::vector<std::string> &words_list, std::map<std::string, int> &dictionary, int start, int end) {

    std::string word;
    for (int i=start; i < end; i++) {
        word = words_list[i];
        dictionary[word] += 1;
    }
}

bool compare_strings(const std::pair<std::string, int>& pair1, const std::pair<std::string, int>& pair2) {
    return pair1.first < pair2.first;
}

bool compare_pairs_results(const std::pair<std::string, int>& pair1, const std::pair<std::string, int>& pair2) {
    return pair1.second > pair2.second;
}



template<typename funcT>
int sort_given_comparator(const std::map<std::string, int>& words, const std::string& file_name, funcT comparator) {
    std::vector<std::pair<std::string, int>> words_vector (words.begin(), words.end());
    sort(words_vector.begin(), words_vector.end(), comparator);
    std::ofstream out(file_name);
    if (out.is_open()) {
        for (const auto &word : words_vector) {
            out << word.first << "  " << std::to_string(word.second) << "\n";
        }
        out.close();
        return 0;
    }
    return -1;
}

void trigger_function(int threadN, const std::string &input, const std::string &out_n, const std::string &out_a) {
    //one thread function

    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

    std::ofstream bya(out_a);
    std::ofstream byn(out_n);

    auto tex = file_definer(input) ? read_file_into_memory(input): read_archive( input.c_str() );
    auto txts = slice_text(tex);
    std::map<std::string, int> dick;

    count_words(txts, dick, 0, txts.size());

    sort_given_comparator(std::ref(dick), out_n, compare_pairs_results);
    sort_given_comparator(std::ref(dick), out_a, compare_strings);

    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    std::cout << "Analyzing: " << time_span.count() << "\n";
}

void mergeMaps(std::vector<std::map<std::string, int>> &allMaps) {
    for (int i = 1; i < allMaps.size(); i++) {
        for (const auto &x: allMaps[i]) {
            allMaps[0][x.first] += x.second;
        }
    }
}


void parallel(int threadN, const std::string &input, const std::string &out_n, const std::string &out_a) {

    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

    ///vector of maps
    std::vector<std::map<std::string, int>> allMaps;
    for (int i = 0; i < threadN; i++) {
        std::map<std::string, int> x;
        allMaps.push_back(x);
    }

    ///split words
    std::ofstream bya(out_a);
    std::ofstream byn(out_n);
    auto tex = file_definer(input) ? read_file_into_memory(input): read_archive( input.c_str() );

    ///pass slices to threads
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

    sort_given_comparator(std::ref(allMaps[0]), out_n, compare_pairs_results);
    sort_given_comparator(std::ref(allMaps[0]), out_a, compare_strings);

    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    std::cout << "Analyzing: " << time_span.count() << "\n";
}

///------------------------------------------------------------------------------------------------------------------///
/// MAIN PROGRAM

int main(int argc, char *argv[]) {

    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

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

    if (std::get<3>(configs) == 1) {
        trigger_function(std::get<3>(configs), std::get<0>(configs), std::get<2>(configs), std::get<1>(configs));
    } else {
        parallel(std::get<3>(configs), std::get<0>(configs), std::get<2>(configs), std::get<1>(configs));
    }

    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    std::cout << "Total: " << time_span.count() << "\n";

    return 0;
}
///------------------------------------------------------------------------------------------------------------------///