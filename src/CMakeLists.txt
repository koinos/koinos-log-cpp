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

koinos_add_format_target(log)

add_library(Koinos::log ALIAS log)

install(
  FILES
    ${PROJECT_SOURCE_DIR}/include/koinos/log.hpp
  DESTINATION
    include/koinos
)
