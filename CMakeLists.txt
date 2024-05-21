cmake_minimum_required(VERSION 3.20)
project(up-abstract-transport
        VERSION 0.1.0
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb")

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/zenohc-install/lib/cmake/")
find_package(nlohmann_json REQUIRED)

include_directories(include)

add_subdirectory(lib)
add_subdirectory(test)

# add_library(UpAbstractTransport SHARED src/UpAbstractTransport.cpp)
# target_link_libraries(UpAbstractTransport PRIVATE nlohmann_json::nlohmann_json)

# add_library(zenoh_wrap SHARED src/ZWrap.cpp src/QueryableServer.cpp src/QueryClient.cpp src/SubscriberServer.cpp src/PublishToSubscribers.cpp)
# target_link_libraries(zenoh_wrap PRIVATE zenohc::lib)

# add_executable(rpc_server test/rpc_server.cpp)
# target_link_libraries(rpc_server PRIVATE zenohc::lib zenoh_wrap)

# add_executable(rpc_client test/rpc_client.cpp)
# target_link_libraries(rpc_client PRIVATE zenohc::lib zenoh_wrap)

# add_executable(sub test/sub.cpp)
# target_link_libraries(sub PRIVATE zenohc::lib zenoh_wrap)

# add_executable(pub test/pub.cpp)
# target_link_libraries(pub PRIVATE UpAbstractTransport nlohmann_json::nlohmann_json)
