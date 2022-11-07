//
// Created by zhuochao su on 4/12/22.
//
#include "include/rapidjson/document.h"
#include "parse_file.h"
#include "iostream"
#include "string"
#include <codecvt>
#include "olestem/stemming/english_stem.h"
#include <dirent.h>
#include <set>
#include "algorithm"
#include "stop_word.h"

using namespace std;

stop_word _stop_word;

//convert string to wstring
//from google
wstring parse_file::s2ws(const string& s) {
    setlocale(LC_ALL, "chs");
    const char* _Source = s.c_str();
    size_t _Dsize = s.size() + 1;
    wchar_t *_Dest = new wchar_t[_Dsize];
    wmemset(_Dest, 0, _Dsize);
    mbstowcs(_Dest, _Source, _Dsize);
    wstring result = _Dest;
    delete[]_Dest;
    setlocale(LC_ALL, "C");
    return result;
}

//convert wstring to string
//from google
string parse_file::stows(wstring& ws) {
    string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";
    setlocale(LC_ALL, "chs");
    const wchar_t* _Source = ws.c_str();
    size_t _Dsize = 2 * ws.size() + 1;
    char *_Dest = new char[_Dsize];
    memset(_Dest, 0, _Dsize);
    wcstombs(_Dest, _Source, _Dsize);
    string result = _Dest;
    delete[]_Dest;
    setlocale(LC_ALL, curLocale.c_str());
    return result;
}

//get all the files from given path
//from google
void parse_file::get_all_files(string& path, vector<string>& files) {
    DIR* dirp;
    struct dirent* dp;
    dirp = opendir(path.c_str());
    while ((dp = readdir(dirp)) != nullptr) {
        string file_name = string(dp->d_name);
        if (file_name[0] != '.') {
            files.push_back((path + file_name));
        }
        //cout << (path + string(dp->d_name)) << endl;
    }
    closedir(dirp);
}

//parse the text and uuid from json files
bool parse_file::parse_json(const string& filename, string& text, string& id, vector<string>& orgs, vector<string>& persons) {
    try {
        rapidjson::Document doc;
        ifstream m_file(filename);
        if (!m_file.is_open()) {
            return false;
        }
        string wholeFile;
        getline(m_file, wholeFile);
        m_file.close();

        doc.Parse(wholeFile.c_str());
        text = doc["text"].GetString();
        id = doc["uuid"].GetString();
        for (int i = 0; i < doc["organizations"].Size(); i++) {
            string org = doc["organizations"][i].GetString();
            orgs.push_back(org);
        }
        for (int i = 0; i < doc["entities"]["organizations"].Size(); i++) {
            string org = doc["entities"]["organizations"][i]["name"].GetString();
            orgs.push_back(org);
        }

        for (int i = 0; i < doc["persons"].Size(); i++) {
            string person = doc["persons"][i].GetString();
            persons.push_back(person);
        }

        for (int i = 0; i < doc["entities"]["persons"].Size(); i++) {
            string person = doc["entities"]["persons"][i]["name"].GetString();
            persons.push_back(person);
        }
        // cout << "text: " << text << endl << endl;
        m_file.close();
        return true;
    } catch (...){
        cout << "parse " << filename << " error" << endl;
        return false;
    }
    return true;
}

//to clean up the texts,
void parse_file::split(string& str, const string& splits,
                       vector<string>& res) {
    //remove all the symbol from the texts
    replace(str.begin(), str.end(), ',', ' ');
    replace(str.begin(), str.end(), '.', ' ');
    replace(str.begin(), str.end(), '\t', ' ');
    replace(str.begin(), str.end(), '\r', ' ');
    replace(str.begin(), str.end(), '\n', ' ');
    replace(str.begin(), str.end(), '\"', ' ');
    replace(str.begin(), str.end(), '\\', ' ');
    replace(str.begin(), str.end(), '/', ' ');
    replace(str.begin(), str.end(), ')', ' ');
    replace(str.begin(), str.end(), '(', ' ');
    replace(str.begin(), str.end(), '%', ' ');
    replace(str.begin(), str.end(), '#', ' ');
    replace(str.begin(), str.end(), '$', ' ');
    replace(str.begin(), str.end(), '\'', ' ');
    replace(str.begin(), str.end(), '*', ' ');
    replace(str.begin(), str.end(), '!', ' ');
    replace(str.begin(), str.end(), '+', ' ');
    replace(str.begin(), str.end(), '<', ' ');
    replace(str.begin(), str.end(), '>', ' ');
    replace(str.begin(), str.end(), '\v', ' ');
    replace(str.begin(), str.end(), '\?', ' ');
    replace(str.begin(), str.end(), '\"', ' ');
    replace(str.begin(), str.end(), '[', ' ');
    replace(str.begin(), str.end(), ']', ' ');

    if (str.empty()) return;
    //change the words to stem words
    stemming::english_stem<> StemEnglish;
    //convert string to wstring
    wstring word = s2ws(str);
    StemEnglish(word);
    //convert wstring to string
    string str_local = stows(word);

    //separate the words
    string strs = str_local + splits;
    size_t pos = strs.find(splits);
    int step = splits.size();
    set<string> temp_set;
    while (pos != string::npos) {
        //get the words and insert it to temp_set
        string temp = strs.substr(0, pos);
        temp_set.insert(temp);
        //move to next words
        strs = strs.substr(pos + step, strs.size());
        pos = strs.find(splits);
    }
    res.reserve(temp_set.size());

    //remove the same words
    auto enditer = temp_set.end();
    for (auto iter = temp_set.begin(); iter != enditer; ++iter) {
        string key =  *iter;
        if (!_stop_word.find(key)) {
            res.emplace_back(key);
        }
    }
}

//build the index for matching the word and uuid
bool parse_file::build_index(string& uuid, string& text, vector<string>& org, vector<string>& person, hashMap& m_org_index, hashMap& m_person_index, word_db& m_text_index) {
    string splits = " ";
    vector<string> res;
    split(text, splits, res);

    size_t num = res.size();
    for (int i = 0; i < num; ++i) {
        m_text_index.insert(res[i], uuid);
    }
    m_person_index.insert(uuid, person);
    m_org_index.insert(uuid, org);
    return true;
}

/*//search the word in the tree and find the uuid
bool parse_file::search_word(string& word, word_db& m_index) {
    // cout << "start search word" << endl;
    vector<string> values;
    bool res = m_index.find(word, values);
    // cout << "after find" << endl;
    if (!res) {
        cout << "find " << word << " is null" << endl;
        return false;
    }
    size_t file_size = values.size();
    cout << word << ": " << endl;
    for (int i = 0; i < file_size; ++i) {
        cout << values[i] << " : ";
    }

    return true;
}*/

void parse_file::parse_string(string &word, word_db& m_index) {
    //tolower case
    transform(word.begin(), word.end(), word.begin(), ::tolower);
    //parse the input string
    vector<string> words_list;
    if(word.empty()){
        return;
    }
    char* strs = new char[word.length()+1];
    strcpy(strs, word.c_str());
    char* p = strtok(strs, " ");

    while(p) {
        string s = p;
        words_list.push_back(s);
        p = strtok(nullptr, " ");
    }
    if(words_list.empty()){
        return;
    }
    for(int i =1; i < words_list.size(); i++){
        if(words_list[0]=="and"){
            if(words_list[i] != "person" && words_list[i] != "org" && words_list[i] != "not"){
                cout << words_list[i] << endl;
                //search_word(words_list[i], m_index);
            }
            else if(words_list[i]=="person"){
                cout << words_list[i+1] << endl;
                i++;
            }
            else if(words_list[i]=="org"){
                cout << words_list[i+1] << endl;
                i++;
            }
            else if(words_list[i]=="not"){
                i++;
                i++;

            }
        } else if(words_list[0] == "or"){
            if(words_list[i] != "person" && words_list[i] != "org" && words_list[i] != "not"){
                cout << words_list[i] << endl;
                i++;
            }
            else if(words_list[i]=="person"){
                cout << words_list[i+1] << endl;
                i++;
            }
            else if(words_list[i]=="org"){
                cout << words_list[i+1] << endl;
                i++;
            }
            else if(words_list[i]=="not"){
                i++;
                i++;
            }
        }
    }

    /*auto a = find(words_list.begin(), words_list.end(), "and");
    auto o = find(words_list.begin(), words_list.end(), "or");
    auto per = find(words_list.begin(), words_list.end(), "person");
    auto org = find(words_list.begin(), words_list.end(), "org");*/

}

parse_file::parse_file() = default;

//https://blog.csdn.net/birenxiaofeigg/article/details/115464934
//https://blog.csdn.net/Emery_learning/article/details/120197865