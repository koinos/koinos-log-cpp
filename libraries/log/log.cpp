#include <koinos/log.hpp>
#include <koinos/util.hpp>

#include <algorithm>
#include <random>
#include <string>

#include <boost/log/sinks/basic_sink_backend.hpp>
#include <boost/log/attributes/value_extraction.hpp>
#include <boost/log/attributes/attribute_value.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>

#define TRACE_STRING    "trace"
#define DEBUG_STRING    "debug"
#define INFO_STRING     "info"
#define WARNING_STRING  "warning"
#define ERROR_STRING    "error"
#define FATAL_STRING    "fatal"

#define TIMESTAMP_ATTR  "TimeStamp"
#define SERVICE_ID_ATTR "ServiceID"
#define FILE_ATTR       "File"
#define LINE_ATTR       "Line"
#define SEVERITY_ATTR   "Severity"
#define MESSAGE_ATTR    "Message"

namespace koinos {

template< bool Color >
class console_sink_impl : public boost::log::sinks::basic_formatted_sink_backend< char, boost::log::sinks::synchronized_feeding >
{
   enum class color : uint8_t
   {
      green,
      yellow,
      red
   };

   static std::string colorize( const std::string& s, color c )
   {
      if constexpr ( !Color )
         return s;

      std::string val = "";

      switch ( c )
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
      auto& s     = std::cout;

      auto time = ptime.time_of_day();
      auto date = ptime.date();

      s << date.year() << "-";
      s << std::right << std::setfill( '0' ) << std::setw( 2 ) << date.month().as_number() << "-";
      s << std::right << std::setfill( '0' ) << std::setw( 2 ) << date.day() << " ";
      s << std::right << std::setfill( '0' ) << std::setw( 2 ) << boost::date_time::absolute_value( time.hours() ) << ":";
      s << std::right << std::setfill( '0' ) << std::setw( 2 ) << boost::date_time::absolute_value( time.minutes() ) << ":";
      s << std::right << std::setfill( '0' ) << std::setw( 2 ) << boost::date_time::absolute_value( time.seconds() ) << ".";
      s << std::right << std::setfill( '0' ) << std::setw( 6 ) << boost::date_time::absolute_value( time.fractional_seconds() );

      s << " (" << svc_id << ")";
      s << " [" << file << ":" << line << "] ";
      s << "<";
      switch ( level.get() )
      {
         case log_level::trace:
            s << colorize( TRACE_STRING, color::green );
            break;
         case log_level::debug:
            s << colorize( DEBUG_STRING, color::green );
            break;
         case log_level::info:
            s << colorize( INFO_STRING, color::green );
            break;
         case log_level::warning:
            s << colorize( WARNING_STRING, color::yellow );
            break;
         case log_level::error:
            s << colorize( ERROR_STRING, color::red );
            break;
         case log_level::fatal:
            s << colorize( FATAL_STRING, color::red );
            break;
         default:
            s << colorize( "unknown", color::red );
            break;
      }
      s << ">: " << formatted_string << std::endl;
   }
};

void initialize_logging(
   const boost::filesystem::path& path,
   const std::string& file_pattern,
   const std::string& application_name,
   const std::optional< std::string >& identifier,
   log_level filter_level,
   bool color )
{
   using console_sink       = boost::log::sinks::synchronous_sink< console_sink_impl< false > >;
   using color_console_sink = boost::log::sinks::synchronous_sink< console_sink_impl< true > >;

   std::string id;

   if ( identifier.has_value() )
      id = identifier.value();
   else
      id = random_alphanumeric( 5 );

   std::string service_id = application_name + "." + id;

   if ( color )
      boost::log::core::get()->add_sink( boost::make_shared< color_console_sink >() );
   else
      boost::log::core::get()->add_sink( boost::make_shared< console_sink >() );

   auto file_name = path.string() + "/" + file_pattern;

   boost::log::register_simple_formatter_factory< boost::log::trivial::severity_level, char >( SEVERITY_ATTR );

   // Output message to file, rotates when file reached 1mb or at midnight every day. Each log file
   // is capped at 1mb and total is 20mb
   boost::log::add_file_log(
      boost::log::keywords::file_name = file_name,
      boost::log::keywords::rotation_size = 1 * 1024 * 1024,
      boost::log::keywords::max_size = 20 * 1024 * 1024,
      boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point( 0, 0, 0 ),
      boost::log::keywords::format = "%" TIMESTAMP_ATTR "% (%" SERVICE_ID_ATTR "%) [%" FILE_ATTR "%:%" LINE_ATTR "%] <%" SEVERITY_ATTR "%>: %" MESSAGE_ATTR "%",
      boost::log::keywords::auto_flush = true
   );

   boost::log::add_common_attributes();
   boost::log::core::get()->add_global_attribute( SERVICE_ID_ATTR, boost::log::attributes::constant< std::string >( service_id ) );

   boost::log::core::get()->set_filter( boost::log::trivial::severity >= filter_level );
}

std::istream& operator>>( std::istream &in, log_level& l )
{
   std::string token;
   in >> token;

   if ( token == TRACE_STRING )
   {
      l = log_level::trace;
   }
   else if ( token == DEBUG_STRING )
   {
      l = log_level::debug;
   }
   else if ( token == INFO_STRING )
   {
      l = log_level::info;
   }
   else if ( token == WARNING_STRING )
   {
      l = log_level::warning;
   }
   else if ( token == ERROR_STRING )
   {
      l = log_level::error;
   }
   else if ( token == FATAL_STRING )
   {
      l = log_level::fatal;
   }
   else
   {
      throw std::runtime_error( "invalid log level" );
   }

   return in;
}

} // koinos
