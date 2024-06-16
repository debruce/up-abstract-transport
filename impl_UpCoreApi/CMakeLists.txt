set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(protobuf REQUIRED)
find_package(up-core-api REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../impl_include)

get_filename_component(IMPL_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

add_library(${IMPL_NAME} SHARED src/UpCoreApi.cpp)

target_link_libraries(${IMPL_NAME} PRIVATE
       	nlohmann_json::nlohmann_json
       	up-core-api::up-core-api
       	protobuf::libprotobuf)

target_include_directories(${IMPL_NAME} PRIVATE
		${up-core-api_INCLUDE_DIR}
		${protobuf_INCLUDE_DIR}
       	)

set_target_properties(${IMPL_NAME} PROPERTIES PREFIX "")
