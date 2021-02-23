file(GLOB HEADERS "include/koinos/*.hpp")

find_package(Boost CONFIG REQUIRED log log_setup filesystem)

add_library(Koinos::log
  log.cpp
  ${HEADERS}
)

target_include_directories(Koinos::log PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(Koinos::log PUBLIC Boost::log_setup Boost::log Boost::filesystem)

install(
  TARGETS Koinos::log
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
INSTALL( FILES ${HEADERS} DESTINATION "include/koinos/chain" )
