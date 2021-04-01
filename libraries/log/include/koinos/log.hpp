#pragma once

#include <filesystem>
#include <optional>
#include <istream>

#include <boost/filesystem/path.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/attributes/mutable_constant.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>

#define LOG(LEVEL)                                                                         \
BOOST_LOG_SEV(::boost::log::trivial::logger::get(), boost::log::trivial::LEVEL)            \
  << boost::log::add_value("Line", __LINE__)                                               \
  << boost::log::add_value("File", boost::filesystem::path(__FILE__).filename().string())  \

namespace koinos {

using log_level = boost::log::trivial::severity_level;

// For use with boost program options
std::istream& operator>>( std::istream &in, log_level& l );

std::filesystem::path get_log_directory( const std::filesystem::path& basedir, const std::string& service );

void initialize_logging(
   const std::filesystem::path& log_directory,
   const std::string& application_name,
   const std::optional< std::string >& identifier = {},
   log_level filter_level = log_level::info,
   const std::string& file_pattern = "%3N.log",
   bool color = true );

} // koinos
