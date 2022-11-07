cmake_minimum_required(VERSION 3.20)
project(22s_final_proj)

set(CMAKE_CXX_STANDARD 17)

add_executable(22s_final_proj main.cpp catch_setup.cpp AVLTree.h test.cpp parse_file.cpp parse_file.h word_db.cpp word_db.h stop_word.h HashNode.cpp HashNode.h hashMap.cpp hashMap.h query_process.cpp query_process.h rank_process.cpp rank_process.h)
