hunter_config(Boost
   VERSION ${HUNTER_Boost_VERSION}
   CMAKE_ARGS
      USE_CONFIG_FROM_BOOST=ON
      Boost_USE_STATIC_LIBS=ON
      Boost_NO_BOOST_CMAKE=ON
)

hunter_config(koinos_util
   URL  "https://github.com/koinos/koinos-util-cpp/archive/bc509581b6859c84bfc8aadeb1ae4cda1bcedd14.tar.gz"
   SHA1 "0bb9ba9f56175360d499e5f6fa4caf59445ee69f"
   CMAKE_ARGS
      BUILD_TESTS=OFF
)

