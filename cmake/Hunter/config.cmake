hunter_config(Boost
   VERSION ${HUNTER_Boost_VERSION}
   CMAKE_ARGS
   USE_CONFIG_FROM_BOOST=ON
   Boost_USE_STATIC_LIBS=ON
   Boost_NO_BOOST_CMAKE=ON
)

hunter_config(koinos_util
   URL "https://github.com/koinos/koinos-util-cpp/archive/32f7589037c6ebcb0c55f2dc558df44ed06176a0.tar.gz"
   SHA1 "958a61b9316556382ac6300f7a40798c9e375040"
   CMAKE_ARGS
      BUILD_TESTS=OFF
)
