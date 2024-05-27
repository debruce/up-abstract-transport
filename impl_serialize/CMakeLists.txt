cmake_minimum_required(VERSION 3.16)
project(lib VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Protobuf REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../impl_include)

file(GLOB ProtoFiles "${CMAKE_CURRENT_SOURCE_DIR}/proto_files/*.proto")
PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders ${ProtoFiles})

message(WARNING "ProtoFiles = ${ProtoFiles}")
message(WARNING "ProtoHeaders = ${ProtoHeaders}")
message(WARNING "ProtoSources = ${ProtoSources}")


add_library(messages STATIC ${ProtoSources} ${ProtoHeaders})
target_link_libraries(messages protobuf::libprotobuf)
target_include_directories(messages PRIVATE "${CMAKE_BINARY_DIR}/impl_serialize")

add_library(impl_serialize SHARED
    src/Impl_serialize.cpp
    )
target_link_libraries(impl_serialize PRIVATE messages nlohmann_json::nlohmann_json)
target_include_directories(impl_serialize PRIVATE "${CMAKE_BINARY_DIR}/impl_serialize")
set_target_properties(impl_serialize PROPERTIES PREFIX "")


