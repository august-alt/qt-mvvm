cmake_minimum_required(VERSION 3.14)
project(qt-mvvm VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
add_compile_options(-stdlib=libc++)
add_link_options(-stdlib=libc++)
endif()

option(MVVM_BUMP_VERSION "Propagate version number" OFF)
option(MVVM_USE_QT6 "Compile with Qt6" ON)
option(MVVM_DISCOVER_TESTS "Auto discover tests and add to ctest, otherwise will run at compile time" ON)
option(MVVM_ENABLE_FILESYSTEM "Enable <filesystem> (requires modern compiler), otherwise rely on Qt" ON)
option(MVVM_BUILD_TESTS "Build tests" OFF)
option(MVVM_BUILD_EXAMPLES "Build user examples" ON)
option(MVVM_SETUP_CLANGFORMAT "Setups target to beautify the code with 'make clangformat'" OFF)
option(MVVM_SETUP_CODECOVERAGE "Setups target to generate coverage information with 'make coverage'" OFF)
option(MVVM_USE_SYSTEM_QCUSTOMPLOT "Use QCustomPlot from linux distribution" OFF)

find_program(NINJA_PROGRAM ninja)
if(NINJA_PROGRAM)
    message(STATUS "Found ninja: ${NINJA_PROGRAM}.")
endif()

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    message(STATUS "Found ccache: ${CCACHE_PROGRAM}.")
endif()

if(CMAKE_C_COMPILER_LAUNCHER)
    message(STATUS "CMAKE_C_COMPILER_LAUNCHER set: ${CMAKE_C_COMPILER_LAUNCHER}.")
endif()

if(CMAKE_CXX_COMPILER_LAUNCHER)
    message(STATUS "CMAKE_CXX_COMPILER_LAUNCHER set: ${CMAKE_CXX_COMPILER_LAUNCHER}.")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/modules)
include(configuration)

message(STATUS "Building for: ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR}")

if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    message(STATUS "MSVC: C VERSION=${CMAKE_C_COMPILER_VERSION} ${CMAKE_C_COMPILER_ARCHITECTURE_ID}")
    message(STATUS "MSVC: C COMPILER=${CMAKE_C_COMPILER}")
    message(STATUS "MSVC: CMAKE_C_FLAGS=${CMAKE_C_FLAGS}")
    message(STATUS "MSVC: CMAKE_C_FLAGS_DEBUG=${CMAKE_C_FLAGS_DEBUG}")
    message(STATUS "MSVC: CMAKE_C_FLAGS_RELEASE=${CMAKE_C_FLAGS_RELEASE}")
    message(STATUS "MSVC: CMAKE_C_FLAGS_MINSIZEREL=${CMAKE_C_FLAGS_MINSIZEREL}")
    message(STATUS "MSVC: CMAKE_C_FLAGS_RELWITHDEBINFO=${CMAKE_C_FLAGS_RELWITHDEBINFO}")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message(STATUS "MSVC: CXX VERSION=${CMAKE_CXX_COMPILER_VERSION} ARCHITECTURE=${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}")
    message(STATUS "MSVC: CXX COMPILER=${CMAKE_CXX_COMPILER}")
    message(STATUS "MSVC: CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")
    message(STATUS "MSVC: CMAKE_CXX_FLAGS_DEBUG=${CMAKE_CXX_FLAGS_DEBUG}")
    message(STATUS "MSVC: CMAKE_CXX_FLAGS_RELEASE=${CMAKE_CXX_FLAGS_RELEASE}")
    message(STATUS "MSVC: CMAKE_CXX_FLAGS_MINSIZEREL=${CMAKE_CXX_FLAGS_MINSIZEREL}")
    message(STATUS "MSVC: CMAKE_CXX_FLAGS_RELWITHDEBINFO=${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
endif()

# https://github.com/cpp-best-practices/cmake_template/blob/main/cmake/CompilerWarnings.cmake

if((CMAKE_C_COMPILER_ID STREQUAL "MSVC") OR (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC"))

    # Set /MP (Build with multiple processes):
    # https://learn.microsoft.com/en-us/cpp/build/reference/mp-build-with-multiple-processes
    add_compile_options(/MP)

    # Warning level:
    add_compile_options(/W4)
    # Note: /W3 is automatically added to the default set of compiler flags on CMake versions previous to 3.15.
    string(REGEX REPLACE "/W[1-3]" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    string(REGEX REPLACE "/W[1-3]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

else()

    # Warning level:
    add_compile_options(-Wall -Wextra -Wpedantic)

endif()

add_subdirectory(thirdparty)
add_subdirectory(source)

if(MVVM_BUILD_TESTS)
    add_subdirectory(tests)
endif(MVVM_BUILD_TESTS)

include(installation)
add_subdirectory(examples)

# Defines 'codecoverage', 'clangformat' targets. See CodeTools.cmake for details.
project_codetools_setup()
