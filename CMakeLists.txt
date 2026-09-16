cmake_minimum_required(VERSION 3.16)
project(attitude-estimator-cpp
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler warnings as errors (rigor de aviónica)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        -Wconversion
        -Wshadow
        -Wnon-virtual-dtor
    )
endif()

# Core Library
include_directories(include)

add_library(attitude_estimator_core STATIC
    src/quaternion_math.cpp
    src/sensor_model.cpp
    src/ekf_core.cpp
    src/attitude_estimator.cpp
)

target_include_directories(attitude_estimator_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
# Testing with Google Test via FetchContent
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP true
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(unit_tests
    tests/test_quaternion_math.cpp
)

target_link_libraries(unit_tests PRIVATE
    attitude_estimator_core
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests)
