#include <boost/algorithm/string.hpp>
#include <boost/test/unit_test.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <koinos/log.hpp>

std::stringbuf buffer;

struct log_fixture
{
  std::streambuf* clog_buffer;

  log_fixture()
  {
    buffer.str( "" );
    clog_buffer = std::clog.rdbuf();
  }

  ~log_fixture()
  {
    boost::log::core::get()->remove_all_sinks();
    std::clog.rdbuf( clog_buffer );
  }
};

BOOST_FIXTURE_TEST_SUITE( log_tests, log_fixture )

BOOST_AUTO_TEST_CASE( log_color_tests )
{
  BOOST_TEST_MESSAGE( "Testing logging library with color" );
  auto buf = std::clog.rdbuf( &buffer );

  std::vector< std::string > logtypes{ "<\033[34mtrace\033[0m>",
                                       "<\033[34mdebug\033[0m>",
                                       "<\033[32minfo\033[0m>",
                                       "<\033[33mwarning\033[0m>",
                                       "<\033[31merror\033[0m>",
                                       "<\033[31mfatal\033[0m>" };

  auto temp = std::filesystem::temp_directory_path() / "log";
  koinos::initialize_logging( "log_test_color", {}, "trace", temp, true );

  LOG( trace ) << "test";
  LOG( debug ) << "test";
  LOG( info ) << "test";
  LOG( warning ) << "test";
  LOG( error ) << "test";
  LOG( fatal ) << "test";

  auto file_path = temp / "log_test_color.log";
  std::ifstream file( file_path.string() );
  BOOST_REQUIRE( file.is_open() );

  std::vector< std::string > log_lines;
  std::string line;

  while( std::getline( file, line ) )
    log_lines.push_back( line );

  std::string& line_one       = log_lines[ 0 ];
  std::size_t pos             = line_one.find( "<" );
  std::string expected_string = line_one.substr( pos );

  std::vector< std::string > results;
  std::string stream_str = buffer.str();
  boost::split( results, stream_str, boost::is_any_of( "\n" ) );
  results.pop_back();

  // Setting std::cout back to normal
  std::clog.rdbuf( buf );

  BOOST_REQUIRE_EQUAL( "<trace>: test", expected_string );

  BOOST_REQUIRE_EQUAL( results.size(), logtypes.size() );
  for( int i = 0; i < results.size(); i++ )
  {
    pos                         = results[ i ].find( "<" );
    std::string expected_result = results[ i ].substr( pos );
    BOOST_REQUIRE_EQUAL( logtypes[ i ] + ": test", expected_result );
  }
}

BOOST_AUTO_TEST_CASE( log_no_color_tests )
{
  BOOST_TEST_MESSAGE( "Testing logging library without color" );
  auto buf = std::clog.rdbuf( &buffer );

  std::vector< std::string > logtypes{ "<trace>", "<debug>", "<info>", "<warning>", "<error>", "<fatal>" };

  auto temp = std::filesystem::temp_directory_path() / "log";
  koinos::initialize_logging( "log_test", "9abcd", "trace", temp, false );

  LOG( trace ) << "test";
  LOG( debug ) << "test";
  LOG( info ) << "test";
  LOG( warning ) << "test";
  LOG( error ) << "test";
  LOG( fatal ) << "test";

  auto file_path = temp / "log_test.log";
  std::ifstream file( file_path.string() );
  BOOST_REQUIRE( file.is_open() );

  std::vector< std::string > log_lines;
  std::string line;

  while( std::getline( file, line ) )
    log_lines.push_back( line );

  std::string& line_one       = log_lines[ 0 ];
  std::size_t pos             = line_one.find( "<" );
  std::string expected_string = line_one.substr( pos );

  std::vector< std::string > results;
  std::string stream_str = buffer.str();
  boost::split( results, stream_str, boost::is_any_of( "\n" ) );
  results.pop_back();

  // Setting std::cout back to normal
  std::clog.rdbuf( buf );

  BOOST_REQUIRE_EQUAL( "<trace>: test", expected_string );

  BOOST_REQUIRE_EQUAL( results.size(), logtypes.size() );
  for( int i = 0; i < results.size(); i++ )
  {
    pos                         = results[ i ].find( "<" );
    std::string expected_result = results[ i ].substr( pos );
    BOOST_REQUIRE_EQUAL( logtypes[ i ] + ": test", expected_result );
  }
}

BOOST_AUTO_TEST_CASE( log_filter_tests )
{
  BOOST_TEST_MESSAGE( "Testing log filtering" );
  auto buf = std::clog.rdbuf( &buffer );

  std::vector< std::string > logtypes{ "<warning>", "<error>", "<fatal>" };

  auto temp = std::filesystem::temp_directory_path() / "log";
  koinos::initialize_logging( "log_test_filter", "9abcd", "warning", temp, false );

  LOG( trace ) << "test";
  LOG( debug ) << "test";
  LOG( info ) << "test";
  LOG( warning ) << "test";
  LOG( error ) << "test";
  LOG( fatal ) << "test";

  auto file_path = temp / "log_test_filter.log";
  std::ifstream file( file_path.string() );
  BOOST_REQUIRE( file.is_open() );

  std::vector< std::string > log_lines;
  std::string line;

  while( std::getline( file, line ) )
    log_lines.push_back( line );

  std::string& line_one       = log_lines[ 0 ];
  std::size_t pos             = line_one.find( "<" );
  std::string expected_string = line_one.substr( pos );

  std::vector< std::string > results;
  std::string stream_str = buffer.str();
  boost::split( results, stream_str, boost::is_any_of( "\n" ) );
  results.pop_back();

  // Setting std::cout back to normal
  std::clog.rdbuf( buf );

  BOOST_REQUIRE_EQUAL( "<warning>: test", expected_string );

  BOOST_REQUIRE_EQUAL( results.size(), logtypes.size() );
  for( int i = 0; i < results.size(); i++ )
  {
    pos                         = results[ i ].find( "<" );
    std::string expected_result = results[ i ].substr( pos );
    BOOST_REQUIRE_EQUAL( logtypes[ i ] + ": test", expected_result );
  }
}

BOOST_AUTO_TEST_SUITE_END()
