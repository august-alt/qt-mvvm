cmake_minimum_required(VERSION 3.14)
project(qt-mvvm VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
add_compile_options(-stdlib=libc++)
add_link_options(-stdlib=libc++)
endif()

option(MVVM_BUMP_VERSION "Propagate version number" OFF)
option(MVVM_DISCOVER_TESTS "Auto discover tests and add to ctest, otherwise will run at compile time" ON)
option(MVVM_ENABLE_FILESYSTEM "Enable <filesystem> (requires modern compiler), otherwise rely on Qt" ON)
option(MVVM_BUILD_TESTS "Build tests" OFF)
option(MVVM_BUILD_EXAMPLES "Build user examples" ON)
option(MVVM_SETUP_CLANGFORMAT "Setups target to beautify the code with 'make clangformat'" OFF)
option(MVVM_SETUP_CODECOVERAGE "Setups target to generate coverage information with 'make coverage'" OFF)
option(MVVM_USE_SYSTEM_QCUSTOMPLOT "Use QCustomPlot from linux distribution" OFF)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/modules)
include(configuration)

add_subdirectory(source)
add_subdirectory(thirdparty)
add_subdirectory(tests)

include(installation)
add_subdirectory(examples)

# Defines 'codecoverage', 'clangformat' targets. See CodeTools.cmake for details.
project_codetools_setup()
