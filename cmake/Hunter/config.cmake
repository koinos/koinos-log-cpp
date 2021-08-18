hunter_config(Boost
   VERSION "1.72.0-p1"
   CMAKE_ARGS
      USE_CONFIG_FROM_BOOST=ON
      Boost_USE_STATIC_LIBS=ON
      Boost_NO_BOOST_CMAKE=ON
)

hunter_config(koinos_util
   URL  "https://github.com/koinos/koinos-util-cpp/archive/2316ed57cae22170cca5a759a8b539261ac5a6ab.tar.gz"
   SHA1 "db7b2112c2db1e3366ebdd9f773b997ec27694b9"
   CMAKE_ARGS
      BUILD_TESTS=OFF
)

