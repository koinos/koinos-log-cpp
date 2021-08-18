hunter_config(Boost
   VERSION "1.72.0-p1"
   CMAKE_ARGS
      USE_CONFIG_FROM_BOOST=ON
      Boost_USE_STATIC_LIBS=ON
      Boost_NO_BOOST_CMAKE=ON
)

hunter_config(koinos_util
   URL  "https://github.com/koinos/koinos-util-cpp/archive/decd81b5fa79a07d98dd2cc803542c6e45a0e1f3.tar.gz"
   SHA1 "014694af780bd755000092eb9aa335f3d9ab7d0f"
   CMAKE_ARGS
      BUILD_TESTS=OFF
)

