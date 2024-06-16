set(CMAKE_POSITION_INDEPENDENT_CODE ON)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/zenohc-install/lib/cmake/")
find_package(zenohc REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../impl_include)

get_filename_component(IMPL_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

add_library(${IMPL_NAME} SHARED
    src/TransportImpl.cpp
    src/PublisherImpl.cpp
    src/SubscriberImpl.cpp
    src/RpcClientFuture.cpp
    src/RpcServerImpl.cpp
    )
target_link_libraries(${IMPL_NAME} PRIVATE nlohmann_json::nlohmann_json zenohc::lib)
set_target_properties(${IMPL_NAME} PROPERTIES PREFIX "")
# set_target_properties(impl_zenoh PROPERTIES PREFIX "")
# set_target_properties(impl_zenoh PROPERTIES PREFIX "")


