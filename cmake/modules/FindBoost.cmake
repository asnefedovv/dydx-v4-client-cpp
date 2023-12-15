include(FetchContent)

FetchContent_Declare(
        Boost
        GIT_REPOSITORY https://github.com/boostorg/boost.git
        GIT_TAG boost-1.81.0
)

set(CMAKE_CXX_FLAGS_OLD "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "")

FetchContent_MakeAvailable(Boost)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_OLD}")
