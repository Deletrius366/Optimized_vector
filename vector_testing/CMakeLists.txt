cmake_minimum_required(VERSION 3.12)
project(set)

set(CMAKE_CXX_STANDARD 17)

add_executable(vector_testing
               vector_testing.cpp
               counted.h
               counted.cpp
               fault_injection.h
               fault_injection.cpp
               mman.h
               mman.cpp
               gtest/gtest-all.cc
               gtest/gtest.h
               gtest/gtest_main.cc)
