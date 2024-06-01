cmake_minimum_required(VERSION 3.16)
project(lib VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(protobuf REQUIRED)
find_package(up-core-api REQUIRED)
message("up-core-api_INCLUDE_DIR = ${up-core-api_INCLUDE_DIR}")
message("protobuf_INCLUDE_DIR = ${protobuf_INCLUDE_DIR}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../impl_include)

add_library(Impl_UpCoreApi SHARED src/Impl_UpCoreApi.cpp)

target_link_libraries(Impl_UpCoreApi PRIVATE
       	nlohmann_json::nlohmann_json
       	up-core-api::up-core-api
       	protobuf::libprotobuf)

target_include_directories(Impl_UpCoreApi PRIVATE
		${up-core-api_INCLUDE_DIR}
		${protobuf_INCLUDE_DIR}
       	)

set_target_properties(Impl_UpCoreApi PROPERTIES PREFIX "")
