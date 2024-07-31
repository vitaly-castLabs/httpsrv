cmake_minimum_required(VERSION 3.14)

cmake_policy(SET CMP0091 NEW)
if (CMAKE_VERSION VERSION_GREATER "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

project(hss LANGUAGES CXX C VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    add_definitions(-DNDEBUG)
endif()

set(TARGET_NAME httpsrv)

if (APPLE)
    execute_process(COMMAND brew --prefix openssl OUTPUT_VARIABLE BREW_SSL_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(OPENSSL_ROOT_DIR ${BREW_SSL_DIR} CACHE STRING "Path to OpenSSL")
endif()

find_package(OpenSSL "1.1.1" COMPONENTS Crypto SSL REQUIRED)

add_subdirectory(cpp-httplib)

add_executable(${TARGET_NAME} main.cpp)

target_compile_definitions(${TARGET_NAME} PUBLIC CPPHTTPLIB_LISTEN_BACKLOG=1000 CPPHTTPLIB_KEEPALIVE_MAX_COUNT=300 CPPHTTPLIB_THREAD_POOL_COUNT=500 CPPHTTPLIB_KEEPALIVE_TIMEOUT_SECOND=15)
set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
target_link_libraries(${TARGET_NAME} PRIVATE httplib::httplib OpenSSL::SSL OpenSSL::Crypto)
