include(FetchContent)

set(ABSL_ENABLE_INSTALL ON)

FetchContent_Declare(gRPC
        GIT_REPOSITORY https://github.com/grpc/grpc
        GIT_TAG v1.60.0
)

set(CMAKE_CXX_FLAGS_OLD "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "")

FetchContent_MakeAvailable(gRPC)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_OLD}")

FetchContent_GetProperties(gRPC SOURCE_DIR gRPC_SOURCE_DIR)
include(${gRPC_SOURCE_DIR}/third_party/protobuf/cmake/protobuf-generate.cmake)