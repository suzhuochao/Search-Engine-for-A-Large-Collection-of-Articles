//
// Created by zhuochao su on 4/12/22.
//

#ifndef INC_22S_FINAL_PROJ_PARSE_FILE_H
#define INC_22S_FINAL_PROJ_PARSE_FILE_H


#include "word_db.h"
#include <map>
#include "hashMap.h"

class parse_file {
    parse_file();

public:
    static wstring s2ws(const string&);
    static string stows(wstring&);
    static void get_all_files(string& path, vector<string>& files);
    static bool parse_json(const string& filename, string& text, string& id, vector<string>& org, vector<string>& person);
    static bool build_index(string& file_name, string& test, vector<string>& org, vector<string>& person, hashMap&, hashMap&, word_db& m_db);
    static bool search_word(string&, word_db& m_db);
    static void split(string& , const string& pattern, vector<string>& res);
    static void parse_string(string& word, word_db& m_index);
};


#endif //INC_22S_FINAL_PROJ_PARSE_FILE_H
