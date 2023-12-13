if (NOT BUILD_TESTING)
  return()
endif()

include(CTest)

koinos_add_test(
  log_tests
    SOURCES
      log_test.cpp
      main.cpp)

target_link_libraries(
  log_tests
    PRIVATE
      log)

target_include_directories(
  log_tests
    PRIVATE
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/tests>)

koinos_add_format(TARGET log_tests)

koinos_coverage(
  EXECUTABLE
    log_tests
  EXCLUDE
    "./tests/*")
