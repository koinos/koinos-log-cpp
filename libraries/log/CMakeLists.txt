file(GLOB HEADERS "include/koinos/*.hpp")

find_package(Boost CONFIG REQUIRED log log_setup filesystem)

add_library(koinos_log
  log.cpp
  ${HEADERS}
)


#target_include_directories(koinos_log PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(koinos_log PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(koinos_log PUBLIC Boost::log_setup Boost::log Boost::filesystem)
add_library(Koinos::log ALIAS koinos_log)

install(
  TARGETS koinos_log
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
INSTALL( FILES ${HEADERS} DESTINATION "include/koinos/chain" )
