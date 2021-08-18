hunter_config(Boost
   VERSION ${HUNTER_Boost_VERSION}
   CMAKE_ARGS
      USE_CONFIG_FROM_BOOST=ON
      Boost_USE_STATIC_LIBS=ON
      Boost_NO_BOOST_CMAKE=ON
)

hunter_config(koinos_util
   URL  "https://github.com/koinos/koinos-util-cpp/archive/22ebcd097395e1e6035fbec7bb993a44e0eab92d.tar.gz"
   SHA1 "8e51b7d1eac85c413c92b4ffe722a848c6a35520"
   CMAKE_ARGS
      BUILD_TESTS=OFF
)

