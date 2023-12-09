include(CTest)

enable_testing()

koinos_add_test(log_tests
  SOURCES
    log_test.cpp
    main.cpp
)

target_link_libraries(log_tests Koinos::log)
target_include_directories(log_tests PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

koinos_add_format(
  TARGET
    log_tests
)

koinos_coverage(
  EXECUTABLE
    log_tests
  EXCLUDE
    "./tests/*"
)
