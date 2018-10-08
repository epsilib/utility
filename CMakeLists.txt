cmake_minimum_required(VERSION 3.10)

project(epsilib_utility VERSION 0.0.2)

add_library(epsilib_utility INTERFACE)

target_include_directories(epsilib_utility INTERFACE include)

target_compile_features(epsilib_utility INTERFACE cxx_std_11)

add_library(epsilib::utility ALIAS epsilib_utility)
