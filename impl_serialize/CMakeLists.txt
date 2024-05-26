cmake_minimum_required(VERSION 3.16)
project(lib VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(impl_serialize SHARED
    src/Impl_serialize.cpp
    )
target_link_libraries(impl_serialize PRIVATE nlohmann_json::nlohmann_json)
set_target_properties(impl_serialize PROPERTIES PREFIX "")


