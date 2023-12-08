include(CTest)

enable_testing()

koinos_add_test(koinos_log_tests
  SOURCES
    log_test.cpp
    main.cpp
)

koinos_add_format_target(koinos_log_tests)

target_link_libraries(koinos_log_tests Koinos::log)
target_include_directories(koinos_log_tests PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
