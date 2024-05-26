cmake_minimum_required(VERSION 3.16)
project(lib VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/zenohc-install/lib/cmake/")
find_package(zenohc REQUIRED)

add_library(impl_zenoh SHARED
    src/Impl_zenoh.cpp
    src/Impl_zenoh_publisher.cpp
    src/Impl_zenoh_subscriber.cpp
    src/Impl_zenoh_rpc_client.cpp
    src/Impl_zenoh_rpc_server.cpp
    )
target_link_libraries(impl_zenoh PRIVATE nlohmann_json::nlohmann_json zenohc::lib)
set_target_properties(impl_zenoh PROPERTIES PREFIX "")
# set_target_properties(impl_zenoh PROPERTIES PREFIX "")
# set_target_properties(impl_zenoh PROPERTIES PREFIX "")


