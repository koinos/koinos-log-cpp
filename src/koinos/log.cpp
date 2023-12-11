#include <koinos/log.hpp>
#include <koinos/util/hex.hpp>
#include <koinos/util/random.hpp>

#include <iostream>
#include <string>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <boost/log/attributes/attribute_value.hpp>
#include <boost/log/attributes/value_extraction.hpp>
#include <boost/log/sinks/basic_sink_backend.hpp>

#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>

#define TRACE_STRING   "trace"
#define DEBUG_STRING   "debug"
#define INFO_STRING    "info"
#define WARNING_STRING "warning"
#define ERROR_STRING   "error"
#define FATAL_STRING   "fatal"

#define TIMESTAMP_ATTR  "TimeStamp"
#define SERVICE_ID_ATTR "ServiceID"
#define FILE_ATTR       "File"
#define LINE_ATTR       "Line"
#define SEVERITY_ATTR   "Severity"
#define MESSAGE_ATTR    "Message"

namespace koinos {

class KoinosFieldValuePrinter: public google::protobuf::TextFormat::FastFieldValuePrinter
{
public:
  virtual void PrintBytes( const std::string& val,
                           google::protobuf::TextFormat::BaseTextGenerator* generator ) const override
  {
    generator->PrintString( util::to_hex( val ) );
  }
};

template< bool Color, bool DateTime >
class console_sink_impl
    : public boost::log::sinks::basic_formatted_sink_backend< char, boost::log::sinks::synchronized_feeding >
{
  enum class color : uint8_t
  {
    green,
    yellow,
    red,
    blue
  };

  static std::string colorize( const std::string& s, color c )
  {
    if constexpr( !Color )
      return s;

    std::string val = "";

    switch( c )
    {
      case color::green:
        val += "\033[32m";
        break;
      case color::yellow:
        val += "\033[33m";
        break;
      case color::red:
        val += "\033[31m";
        break;
      case color::blue:
        val += "\033[34m";
        break;
    }

    val += s;
    val += "\033[0m";

    return val;
  }

public:
  static void consume( const boost::log::record_view& rec, const string_type& formatted_string )
  {
    auto level  = rec[ boost::log::trivial::severity ];
    auto svc_id = rec.attribute_values()[ SERVICE_ID_ATTR ].extract< std::string >();
    auto line   = rec.attribute_values()[ LINE_ATTR ].extract< int >();
    auto file   = rec.attribute_values()[ FILE_ATTR ].extract< std::string >();
    auto ptime  = rec.attribute_values()[ TIMESTAMP_ATTR ].extract< boost::posix_time::ptime >().get();
    auto& s     = std::clog;

    if constexpr( DateTime )
    {
      auto time = ptime.time_of_day();
      auto date = ptime.date();

      s << date.year() << "-";
      s << std::right << std::setfill( '0' ) << std::setw( 2 ) << date.month().as_number() << "-";
      s << std::right << std::setfill( '0' ) << std::setw( 2 ) << date.day() << " ";
      s << std::right << std::setfill( '0' ) << std::setw( 2 ) << boost::date_time::absolute_value( time.hours() )
        << ":";
      s << std::right << std::setfill( '0' ) << std::setw( 2 ) << boost::date_time::absolute_value( time.minutes() )
        << ":";
      s << std::right << std::setfill( '0' ) << std::setw( 2 ) << boost::date_time::absolute_value( time.seconds() )
        << ".";
      s << std::right << std::setfill( '0' ) << std::setw( 6 )
        << boost::date_time::absolute_value( time.fractional_seconds() );
      s << " ";
    }

    s << "(" << svc_id << ")";
    s << " [" << file << ":" << line << "] ";
    s << "<";
    switch( level.get() )
    {
      case boost::log::trivial::severity_level::trace:
        s << colorize( TRACE_STRING, color::blue );
        break;
      case boost::log::trivial::severity_level::debug:
        s << colorize( DEBUG_STRING, color::blue );
        break;
      case boost::log::trivial::severity_level::info:
        s << colorize( INFO_STRING, color::green );
        break;
      case boost::log::trivial::severity_level::warning:
        s << colorize( WARNING_STRING, color::yellow );
        break;
      case boost::log::trivial::severity_level::error:
        s << colorize( ERROR_STRING, color::red );
        break;
      case boost::log::trivial::severity_level::fatal:
        s << colorize( FATAL_STRING, color::red );
        break;
      default:
        s << colorize( "unknown", color::red );
        break;
    }
    s << ">: " << formatted_string << std::endl;
  }
};

boost::log::trivial::severity_level level_from_string( const std::string& token )
{
  boost::log::trivial::severity_level l;

  if( token == TRACE_STRING )
  {
    l = boost::log::trivial::severity_level::trace;
  }
  else if( token == DEBUG_STRING )
  {
    l = boost::log::trivial::severity_level::debug;
  }
  else if( token == INFO_STRING )
  {
    l = boost::log::trivial::severity_level::info;
  }
  else if( token == WARNING_STRING )
  {
    l = boost::log::trivial::severity_level::warning;
  }
  else if( token == ERROR_STRING )
  {
    l = boost::log::trivial::severity_level::error;
  }
  else if( token == FATAL_STRING )
  {
    l = boost::log::trivial::severity_level::fatal;
  }
  else
  {
    throw std::runtime_error( "invalid log level" );
  }

  return l;
}

void initialize_logging( const std::string& application_name,
                         const std::optional< std::string >& identifier,
                         const std::string& filter_level,
                         const std::optional< std::filesystem::path >& log_directory,
                         bool color,
                         bool datetime )
{
  using console_sink                = boost::log::sinks::synchronous_sink< console_sink_impl< false, false > >;
  using console_datetime_sink       = boost::log::sinks::synchronous_sink< console_sink_impl< false, true > >;
  using color_console_sink          = boost::log::sinks::synchronous_sink< console_sink_impl< true, false > >;
  using color_console_datetime_sink = boost::log::sinks::synchronous_sink< console_sink_impl< true, true > >;

  std::string id;

  if( identifier.has_value() )
    id = identifier.value();
  else
    id = util::random_alphanumeric( 5 );

  std::string service_id = application_name + "." + id;

  if( color )
    if( datetime )
      boost::log::core::get()->add_sink( boost::make_shared< color_console_datetime_sink >() );
    else
      boost::log::core::get()->add_sink( boost::make_shared< color_console_sink >() );
  else if( datetime )
    boost::log::core::get()->add_sink( boost::make_shared< console_datetime_sink >() );
  else
    boost::log::core::get()->add_sink( boost::make_shared< console_sink >() );

  boost::log::register_simple_formatter_factory< boost::log::trivial::severity_level, char >( SEVERITY_ATTR );

  if( log_directory.has_value() )
  {
    // Output message to file, rotates when file reached 1mb. Each log file
    // is capped at 1mb and total is 100mb and 100 files.
    boost::log::add_file_log(
      boost::log::keywords::file_name = log_directory->string() + "/" + application_name + ".log",
      boost::log::keywords::target_file_name =
        log_directory->string() + "/" + application_name + "-%Y-%m-%dT%H-%M-%S.%f.log",
      boost::log::keywords::target        = log_directory->string(),
      boost::log::keywords::rotation_size = 1 * 1024 * 1024,
      boost::log::keywords::max_size      = 100 * 1024 * 1024,
      boost::log::keywords::max_files     = 100,
      boost::log::keywords::format        = "%" TIMESTAMP_ATTR "% (%" SERVICE_ID_ATTR "%) [%" FILE_ATTR "%:%" LINE_ATTR
                                     "%] <%" SEVERITY_ATTR "%>: %" MESSAGE_ATTR "%",
      boost::log::keywords::auto_flush = true );
  }

  boost::log::add_common_attributes();
  boost::log::core::get()->add_global_attribute( SERVICE_ID_ATTR,
                                                 boost::log::attributes::constant< std::string >( service_id ) );

  boost::log::core::get()->set_filter( boost::log::trivial::severity >= level_from_string( filter_level ) );
}

} // namespace koinos

namespace google::protobuf {

std::ostream& operator<<( std::ostream& os, const google::protobuf::Message& m )
{
  google::protobuf::TextFormat::Printer printer;
  printer.SetSingleLineMode( true );
  printer.SetDefaultFieldValuePrinter( new koinos::KoinosFieldValuePrinter() );
  auto ost = google::protobuf::io::OstreamOutputStream( &os );
  printer.Print( m, &ost );
  return os;
}

} // namespace google::protobuf
