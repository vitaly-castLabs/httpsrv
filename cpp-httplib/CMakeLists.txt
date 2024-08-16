include(FetchContent)

FetchContent_Declare(
    cpp-httplib
    GIT_REPOSITORY https://github.com/vitaly-castLabs/cpp-httplib
)

set(HTTPLIB_NO_EXCEPTIONS ON)
set(HTTPLIB_REQUIRE_OPENSSL ON)
set(HTTPLIB_REQUIRE_ZLIB ON)
if (NOT WIN32)
    # it's called unofficial-brotli on Win atm (when installed with
    # vcpkg), needs to be taken care of on the cpp-httplib side
    set(HTTPLIB_REQUIRE_BROTLI ON)
endif()
FetchContent_MakeAvailable(cpp-httplib)
