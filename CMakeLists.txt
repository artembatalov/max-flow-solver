cmake_minimum_required(VERSION 3.16)
project(max_flow_solver VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-Wall -Wextra -Wpedantic)

include_directories(include)

add_library(maxflow
    src/edmonds_karp.cpp
    src/dinic.cpp
)
target_include_directories(maxflow PUBLIC include)

add_executable(example_problem15 examples/problem15.cpp)
target_link_libraries(example_problem15 maxflow)

add_executable(example_realworld examples/realworld.cpp)
target_link_libraries(example_realworld maxflow)

add_executable(benchmark benchmarks/benchmark.cpp)
target_link_libraries(benchmark maxflow)

enable_testing()
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(tests
    tests/test_edmonds_karp.cpp
    tests/test_dinic.cpp
    tests/test_mincut.cpp
    tests/test_decomposition.cpp
)
target_link_libraries(tests maxflow GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(tests)
