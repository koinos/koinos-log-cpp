hunter_config(Boost
   VERSION "1.72.0-p1"
   CMAKE_ARGS
      USE_CONFIG_FROM_BOOST=ON
      Boost_USE_STATIC_LIBS=ON
      Boost_NO_BOOST_CMAKE=ON
)

hunter_config(koinos_util
   URL  "https://github.com/koinos/koinos-util-cpp/archive/0278dbb73554fbf8f9cd1c9664a2c6e793f4638f.tar.gz"
   SHA1 "11a89bc90cb446b00281d8234916af81f5172a56"
   CMAKE_ARGS
      BUILD_TESTS=OFF
)

