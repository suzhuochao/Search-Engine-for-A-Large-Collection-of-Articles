#include <iostream>
#include "string"
#include "catch_setup.h"
#include "parse_file.h"
#include "query_process.h"
#include <chrono>

using namespace std;

word_db m_index;
vector<string> files;

int main(int argc, char** argv) {
    if(argc == 0){
        runCatchTests();
    } else {
        query_process process;
        /*//string words = argv[1];
        string dataFile = argv[2];
        //cout << words << " " << dataFile;
        using namespace chrono;
        steady_clock::time_point t1 = steady_clock::now();

        // string m_file = "blogs_0000001.json";
        // string text;
        //string m_path = "./data/2018_01_112b52537b67659ad3609a234388c50a/";
        string words = "OR facebook instagram NOT bankruptcy ORG snap PERSON cramer";
        parse_file::parse_string(words, m_index);
        exit(1);
        parse_file::get_all_files(dataFile, files);
        for (auto &file: files) {
            string id;
            string text;
            //parse_file::parse_json(file, text, id);
            parse_file::build_index(id, text, m_index);
        }
        steady_clock::time_point t2 = steady_clock::now();

        string search_words = words;
        parse_file::search_word(search_words, m_index);

        steady_clock::time_point t3 = steady_clock::now();
        duration<double> time_span1 = duration_cast<duration<double>>(t2 - t1);
        cout << "It took me " << time_span1.count() << "seconds. to build index" << endl;
        duration<double> time_span2 = duration_cast<duration<double>>(t3 - t2);
        cout << "It took me " << time_span2.count() << "seconds. to search word" << endl;
*/
        return 0;
    }
}