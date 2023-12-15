include(FetchContent)

FetchContent_Declare(protobuf
        GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
        GIT_TAG v25.1
)

FetchContent_MakeAvailable(protobuf)

FetchContent_GetProperties(protobuf SOURCE_DIR protobuf_SOURCE_DIR)
include(${protobuf_SOURCE_DIR}/cmake/protobuf-generate.cmake)