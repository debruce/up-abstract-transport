set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(OpenSSL 3.2.1 REQUIRED)
find_package(Threads)
find_package(protobuf REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../impl_include)

add_library(UpAbstractTransport SHARED
    src/TransportBase.cpp
    src/Concepts.cpp
    src/Serializer.cpp
    src/AnyMap.cpp
    src/DllHandle.cpp)

target_link_libraries(UpAbstractTransport
    PUBLIC
    nlohmann_json::nlohmann_json
    ${CMAKE_DL_LIBS}
    Threads::Threads
    OpenSSL::Crypto
    protobuf::libprotobuf
    )

target_include_directories(UpAbstractTransport PRIVATE
    ${protobuf_INCLUDE_DIR}
    )

