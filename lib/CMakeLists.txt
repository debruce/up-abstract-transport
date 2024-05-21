cmake_minimum_required(VERSION 3.16)
project(lib VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# if(BUILD_UNBUNDLED)
#     find_package(zenohc REQUIRED)
#     set(ZENOH_LIBRARY zenohc::lib)
# else()
#     if(CROSS_COMPILE)
#         find_package(zenohc REQUIRED)
# 	set(ZENOH_LIBRARY zenohc::lib)
#     else()
#         find_library(ZENOH_LIB zenohc)
# 	set(ZENOH_LIBRARY ${ZENOH_LIB})
#     endif()
# endif()

# find_package(GTest REQUIRED)

add_library(UpAbstractTransport SHARED src/UpAbstractTransport.cpp)
target_link_libraries(UpAbstractTransport PRIVATE nlohmann_json::nlohmann_json)

# target_link_libraries(PluginApi PRIVATE ${DL_LIBRARY} OpenSSL::Crypto)

