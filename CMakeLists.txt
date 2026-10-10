cmake_minimum_required(VERSION 3.10.0)
project(JobScheduler VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB source_files "src/*.cpp")

add_library(event_bus STATIC ${source_files})

target_include_directories(event_bus
    PUBLIC
    include
)

set_target_properties(event_bus PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_compile_features(event_bus PUBLIC cxx_std_23)

option(event_bus_build_tests "Build Event Bus tests" ${PROJECT_IS_TOP_LEVEL})
if (event_bus_build_tests)
    add_subdirectory(tests)
endif()