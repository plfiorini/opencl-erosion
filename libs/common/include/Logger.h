#pragma once
#ifndef __LOGGER_H__
#define __LOGGER_H__

/**
 * Basic Idea for Logging Class from Petru Marginean's Article
 * for more information see
 *  http://www.drdobbs.com/cpp/201804215
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <cstdint>
#include <cstddef>

#include "Log_level.h"

namespace mkay
{
  typedef std::basic_ostream<char, std::char_traits<char> > Log_ostream;

  class Logger
  {
  public:
    Logger() {}
    ~Logger();

    std::ostringstream &get( Log_level level
                           , char const * i_pFilePath
                           , char const * i_pFunctionName
                           , int32_t const i_LineNo );

    static void set_write_time_prefix( bool i_write_time_prefix ) { ms_write_time_prefix = i_write_time_prefix; }

    static Log_level get_verbosity() { return ms_verbosity; }
    
    /// set the logging verbosity, all messages with level <= verbosity are logged
    static void set_verbosity(Log_level i_verbosity) { ms_verbosity = i_verbosity; }

    static void add_logging_destination(Log_level i_level, Log_ostream *i_logging_destination) 
    {
      ms_logging_destinations[i_level].push_back(i_logging_destination); 
    }
    static void add_logging_destination(Log_level i_level, std::ofstream *i_logging_destination) 
    {
      ms_logging_destinations[i_level].push_back(reinterpret_cast<Log_ostream *>(i_logging_destination)); 
    }

    /***
     * Init Logging with defaults:
     *  - a logfile (if not null)
     *  - log err&fatal to cerr, all other things to cout
     */
    static void init_defaults(std::ofstream *i_log_file = nullptr, bool i_write_time_prefix = false);

  protected:
    std::ostringstream m_output_stream;

    static bool ms_write_time_prefix;
    static Log_level ms_verbosity;
    static std::map<Log_level, std::vector<Log_ostream *> > ms_logging_destinations;

  private:
    Logger(const Logger&);
    Logger& operator=(const Logger&);

    Log_level m_message_level;

    void write_prefix(char const * i_file_path, char const * i_function_name, int32_t const i_line_no);
  };

} // namespace mkay
  
// convenience macros
#define LOG(level)                        \
  if( level <= Logger::get_verbosity() )     \
    Logger().get(level, __FILE__, __FUNCTION__, __LINE__) 

#define logdeb    LOG(Log_level::Debug)
#define logdet    LOG(Log_level::Detail)
#define loginf    LOG(Log_level::Info)
#define logwarn   LOG(Log_level::Warning)
#define logerr    LOG(Log_level::Error)
#define logfat    LOG(Log_level::Fatal)

// include log util functions
#include "Log_util_func.h"

#endif // __LOGGER_H__
