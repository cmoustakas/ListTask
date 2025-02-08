cmake_minimum_required(VERSION 3.10)
project(task_list)

set(CMAKE_CXX_STANDARD 17)

# Enable testing
enable_testing()

# Add Google Test
add_subdirectory(third_party/googletest)
include_directories(third_party/googletest/googletest/include)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

# Source files
set(TASK_LIST_SOURCES)
list(APPEND TASK_LIST_SOURCES
    src/Solver.cpp
    include/Solver.hpp
    src/SimilarityUtil.cpp
    include/SimilarityUtil.hpp
    include/Entity.hpp
)

# Add main library
add_library(${PROJECT_NAME} STATIC ${TASK_LIST_SOURCES})

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    include
)

# Google Test executable for unit tests
add_executable(task_list_tests third_party/unit_tests/solver_tests.cpp)
target_link_libraries(task_list_tests PRIVATE ${PROJECT_NAME} gtest gtest_main)
