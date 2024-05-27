cmake_minimum_required(VERSION 3.16)
project(lib VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set( DL_LIBRARY /usr/lib/x86_64-linux-gnu/libdl.so.2 )
find_package(OpenSSL REQUIRED)
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../impl_include)

add_library(UpAbstractTransport SHARED src/UpAbstractTransport.cpp src/DllHandle.cpp)
target_link_libraries(UpAbstractTransport PUBLIC nlohmann_json::nlohmann_json PRIVATE ${DL_LIBRARY} OpenSSL::Crypto)

# target_link_libraries(PluginApi PRIVATE ${DL_LIBRARY} OpenSSL::Crypto)

