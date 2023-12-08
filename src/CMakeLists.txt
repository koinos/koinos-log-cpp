add_library(log
  log.cpp

  ${PROJECT_SOURCE_DIR}/include/koinos/log.hpp)

target_link_libraries(
  log
    PUBLIC
      Boost::log_setup
      Boost::log
      Koinos::util
      protobuf::libprotobuf
)

target_include_directories(
  log
    PUBLIC
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>
)

add_library(Koinos::log ALIAS log)

koinos_add_format(
  TARGET
    log
)

koinos_install(TARGETS log)

install(
  DIRECTORY
    ${PROJECT_SOURCE_DIR}/include
  DESTINATION
    ${CMAKE_INSTALL_PREFIX}
)
