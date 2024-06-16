set(CMAKE_POSITION_INDEPENDENT_CODE ON)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/zenohc-install/lib/cmake/")
find_package(zenohc REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../impl_include)

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


