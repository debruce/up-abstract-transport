cmake_minimum_required(VERSION 3.20)
project(up-abstract-transport
        VERSION 0.1.0
        LANGUAGES CXX)

cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb")

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/zenohc-install/lib/cmake/")
set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build")
find_package(nlohmann_json REQUIRED)

include_directories(include)

add_subdirectory(lib)
add_subdirectory(test)
add_subdirectory(impl_Zenoh)
add_subdirectory(impl_UpCoreApi)
#add_subdirectory(impl_UdpSocket)
