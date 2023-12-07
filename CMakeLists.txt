
cmake_minimum_required(VERSION 3.19.0)

cmake_policy(SET CMP0074 NEW)
cmake_policy(SET CMP0114 NEW)

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.24.0)
  cmake_policy(SET CMP0135 NEW)
endif()

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.27.0)
  cmake_policy(SET CMP0144 NEW)
endif()

include(FetchContent)
FetchContent_Declare(
  koinos_cmake
  GIT_REPOSITORY https://github.com/koinos/koinos-cmake.git
  GIT_TAG        f3d5dd68611a515837b2a038cc93365ad12e8c41
)
FetchContent_MakeAvailable(koinos_cmake)

include("${koinos_cmake_SOURCE_DIR}/Koinos.cmake")

project(koinos_log
   VERSION 2.0.0
   DESCRIPTION "The Koinos log library"
   LANGUAGES CXX C)

koinos_coverage(
  EXECUTABLE
    koinos_log_tests
  EXCLUDE
    "tests/*"
)

koinos_add_package(Boost CONFIG REQUIRED
  ADD_COMPONENTS program_options test log
  FIND_COMPONENTS program_options log log_setup)

koinos_add_package(Protobuf)

koinos_add_package(koinos_util)

add_subdirectory(src)
add_subdirectory(tests)

koinos_install(TARGETS log)

