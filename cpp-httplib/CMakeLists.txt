include(FetchContent)

FetchContent_Declare(
    cpp-httplib
    GIT_REPOSITORY https://github.com/vitaly-castLabs/cpp-httplib
)

set(HTTPLIB_NO_EXCEPTIONS ON)
set(HTTPLIB_REQUIRE_OPENSSL ON)
set(HTTPLIB_REQUIRE_ZLIB ON)
set(HTTPLIB_REQUIRE_BROTLI ON)
FetchContent_MakeAvailable(cpp-httplib)
