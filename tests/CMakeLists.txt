
add_executable(event_bus_tests main.cpp)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_compile_options(event_bus_tests PRIVATE -stdlib=libc++)
    target_link_options(event_bus_tests PRIVATE -stdlib=libc++ -lc++abi)
endif()

target_link_libraries(event_bus_tests PRIVATE event_bus)