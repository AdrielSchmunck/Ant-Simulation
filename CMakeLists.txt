cmake_minimum_required (VERSION 3.8)

# Project orbitalsim
project(antcolony)

# raylib-cpp
find_package(raylib_cpp QUIET)
if (NOT raylib_cpp_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib_cpp
        GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
        GIT_TAG v4.1.0
    )
    FetchContent_MakeAvailable(raylib_cpp)
endif()

# raylib: required for macOS
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_CXX_STANDARD 11)
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif()

# Main executable
add_executable(antcolony1 main.cpp ant.cpp "obstacle.cpp")

# Raylib
find_package(raylib CONFIG REQUIRED)
target_include_directories(antcolony1 PRIVATE ${RAYLIB_INCLUDE_DIRS})
target_link_libraries(antcolony1 PRIVATE ${RAYLIB_LIBRARIES}  raylib_cpp)


 #Main test
#enable_testing()

#add_executable(antcolony_test main_test.cpp "ant.cpp" )

#add_test(NAME test1 COMMAND antcolony_test)

#target_include_directories(antcolony_test PRIVATE ${RAYLIB_INCLUDE_DIRS})
#target_link_libraries(antcolony_test PRIVATE ${RAYLIB_LIBRARIES}  raylib_cpp)

# raylib: required for macOS
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_link_libraries(antcolony PRIVATE "-framework IOKit")
    target_link_libraries(antcolony PRIVATE "-framework Cocoa")
    target_link_libraries(antcolony PRIVATE "-framework OpenGL")
endif()


