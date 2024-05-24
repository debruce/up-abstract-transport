cmake_minimum_required(VERSION 3.16)
project(test VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

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

# add_executable(pub src/pub.cpp)
# target_link_libraries(pub PUBLIC UpAbstractTransport PRIVATE nlohmann_json::nlohmann_json)

add_executable(pub_test src/pub_test.cpp)
target_link_libraries(pub_test PUBLIC UpAbstractTransport PRIVATE nlohmann_json::nlohmann_json)

add_executable(sub_test src/sub_test.cpp)
target_link_libraries(sub_test PUBLIC UpAbstractTransport PRIVATE nlohmann_json::nlohmann_json)

add_executable(rpc_server_test src/rpc_server_test.cpp)
target_link_libraries(rpc_server_test PUBLIC UpAbstractTransport PRIVATE nlohmann_json::nlohmann_json)

# add_executable(api_test src/api_test.cpp)
# target_link_libraries(api_test PUBLIC PluginApi)

# add_executable(pub_test src/pub_test.cpp)
# target_link_libraries(pub_test PUBLIC PluginApi)



# add_executable(rpc_client_test src/rpc_client_test.cpp)
# target_link_libraries(rpc_client_test PUBLIC PluginApi)

# add_executable(rpc_server_test src/rpc_server_test.cpp)
# target_link_libraries(rpc_server_test PUBLIC PluginApi)