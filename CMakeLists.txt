cmake_minimum_required(VERSION 3.10)

project(epsilib-utility VERSION 0.0.1)

add_library(epsilib-utility
    include/epsilib/utility.hpp
    include/epsilib/utility/container_helpers.hpp
    include/epsilib/utility/to_string.hpp
    include/epsilib/utility/tictoc.hpp
    src/tictoc.cpp
)

add_library(epsilib::utility ALIAS epsilib-utility)

target_include_directories(epsilib-utility PUBLIC include)

target_compile_features(epsilib-utility PUBLIC cxx_std_11)
