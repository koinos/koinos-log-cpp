
cmake_minimum_required(VERSION 3.19.0)

cmake_policy(VERSION 3.19.0...3.27.4)

include(FetchContent)
FetchContent_Declare(
  koinos_cmake
  GIT_REPOSITORY https://github.com/koinos/koinos-cmake.git
  GIT_TAG        897f11188f1761ddd810d77839417549e68465aa)
FetchContent_MakeAvailable(koinos_cmake)

include("${koinos_cmake_SOURCE_DIR}/Koinos.cmake")

project(koinos_log
  VERSION 2.0.0
  DESCRIPTION "The Koinos log library"
  LANGUAGES CXX C)

koinos_define_version()

koinos_add_package(Boost CONFIG REQUIRED
  ADD_COMPONENTS program_options test log
  FIND_COMPONENTS program_options log log_setup)

koinos_add_package(Protobuf)
koinos_add_package(yaml-cpp CONFIG REQUIRED)

koinos_add_package(koinos_util)

add_subdirectory(src)
add_subdirectory(tests)

