cmake_minimum_required(VERSION 3.24)

project(kalcy VERSION 0.2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(project_prefix kalcy)

set(is_root_project OFF)

if("${CMAKE_PROJECT_NAME}" STREQUAL "${PROJECT_NAME}")
  set(is_root_project ON)
endif()

option(KALCY_BUILD_EXAMPLES "Build kalcy examples" ${is_root_project})
option(KALCY_BUILD_TESTS "Build kalcy tests" ${is_root_project})

add_library(kalcy-compile-options INTERFACE)
add_library(${project_prefix}::kalcy-compile-options ALIAS kalcy-compile-options)

if(CMAKE_CXX_COMPILER_ID STREQUAL Clang OR CMAKE_CXX_COMPILER_ID STREQUAL GNU)
  target_compile_options(kalcy-compile-options INTERFACE
    -Wall -Wextra -Wpedantic -Wconversion -Wunused -Werror=return-type $<$<NOT:$<CONFIG:Debug>>:-Werror>
  )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL MSVC)
  target_compile_options(kalcy-compile-options INTERFACE
    $<$<NOT:$<CONFIG:Debug>>:/WX>
  )
endif()

add_subdirectory(kalcy)

if(KALCY_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

if(KALCY_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
