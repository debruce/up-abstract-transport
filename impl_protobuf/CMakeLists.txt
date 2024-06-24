find_package(protobuf REQUIRED)
find_package(up-core-api REQUIRED)

add_executable(proto_test src/proto_test.cpp)

target_link_libraries(proto_test PRIVATE
       	up-core-api::up-core-api
       	protobuf::libprotobuf)

target_include_directories(proto_test PRIVATE
		${up-core-api_INCLUDE_DIR}
		${protobuf_INCLUDE_DIR}
       	)

