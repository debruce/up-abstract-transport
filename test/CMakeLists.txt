add_executable(load_test src/load_test.cpp)
target_link_libraries(load_test PUBLIC UpAbstractTransport PRIVATE nlohmann_json::nlohmann_json)

add_executable(pub_test src/pub_test.cpp)
target_link_libraries(pub_test PUBLIC UpAbstractTransport PRIVATE nlohmann_json::nlohmann_json)

add_executable(sub_test src/sub_test.cpp)
target_link_libraries(sub_test PUBLIC UpAbstractTransport PRIVATE nlohmann_json::nlohmann_json)

add_executable(rpc_client_test src/rpc_client_test.cpp)
target_link_libraries(rpc_client_test PUBLIC UpAbstractTransport PRIVATE nlohmann_json::nlohmann_json)

add_executable(rpc_server_test src/rpc_server_test.cpp)
target_link_libraries(rpc_server_test PUBLIC UpAbstractTransport PRIVATE nlohmann_json::nlohmann_json)

add_executable(serializer_test src/serializer_test.cpp)
target_link_libraries(serializer_test PUBLIC UpAbstractTransport PRIVATE nlohmann_json::nlohmann_json)
