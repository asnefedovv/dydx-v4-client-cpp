Include(FetchContent)

set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(
        bip3x
        GIT_REPOSITORY https://github.com/edwardstock/bip3x.git
        GIT_TAG 3.0.0
        GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(bip3x)
