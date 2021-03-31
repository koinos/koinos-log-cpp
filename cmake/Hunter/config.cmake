hunter_config(Boost
   VERSION ${HUNTER_Boost_VERSION}
   CMAKE_ARGS
   USE_CONFIG_FROM_BOOST=ON
   Boost_USE_STATIC_LIBS=ON
   Boost_NO_BOOST_CMAKE=ON
)

hunter_config(koinos_util
   GIT_SUBMODULE "libraries/util"
   CMAKE_ARGS
      BUILD_TESTS=OFF
)

