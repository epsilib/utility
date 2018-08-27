cmake_minimum_required(VERSION 3.10)

project(epsilib-utility VERSION 0.0.2)

add_library(epsilib-utility INTERFACE)

add_library(epsilib::utility ALIAS epsilib-utility)

target_include_directories(epsilib-utility INTERFACE include)

target_compile_features(epsilib-utility INTERFACE cxx_std_11)
