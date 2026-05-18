cmake_minimum_required(VERSION 3.10)
project(HackathonSquad VERSION 1.0 LANGUAGES CXX)

# Set C++ standard to C++20 for modern features (like std::chrono, std::span, ranges)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Maximum optimizations for speed!
# -O3: Heaviest optimizations
# -march=native: Use instructions specific to the CPU compiling it
# -DNDEBUG: Turn off asserts for max speed
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")

# Define the include directories so we don't have to write long paths
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/core/graph
    ${CMAKE_CURRENT_SOURCE_DIR}/core/state
    ${CMAKE_CURRENT_SOURCE_DIR}/solver/reductions
    ${CMAKE_CURRENT_SOURCE_DIR}/solver/memetic
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/timer
)

# Collect all our source files
set(SOURCES
    core/graph/graph.cpp
    core/state/state.cpp
    solver/reductions/reductions.cpp
    solver/memetic/memetic.cpp
)

# The main CLI executable
add_executable(solver main.cpp ${SOURCES})

# The benchmark tool executable
add_executable(benchmark_runner evaluator/benchmark/benchmark_runner.cpp ${SOURCES})
