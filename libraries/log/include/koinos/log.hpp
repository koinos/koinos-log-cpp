#pragma once

#include <filesystem>
#include <optional>
#include <ostream>
#include <istream>

#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/attributes/mutable_constant.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>

#include <google/protobuf/message.h>

namespace google::protobuf
{
   std::ostream& operator<<( std::ostream& os, const google::protobuf::Message& m );
}

#define LOG(LEVEL)                                                                         \
BOOST_LOG_SEV(::boost::log::trivial::logger::get(), boost::log::trivial::LEVEL)            \
  << boost::log::add_value("Line", __LINE__)                                               \
  << boost::log::add_value("File", boost::filesystem::path(__FILE__).filename().string())  \

namespace koinos {

void initialize_logging(
   const std::string& application_name,
   const std::optional< std::string >& identifier = {},
   const std::string& filter_level = "info",
   const std::optional< std::filesystem::path >& log_directory = {},
   const std::string& file_pattern = "%3N.log",
   bool color = true );

} // koinos
