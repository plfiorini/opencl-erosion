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

#include <boost/cstdint.hpp>

#include "LogLevel.h"

typedef std::basic_ostream<char, std::char_traits<char> > LogOstream;

class Log
{
public:
  Log();
  ~Log();

  std::ostringstream &Get( LogLevel level, 
                           char const * i_pFilePath,
                           char const * i_pFunctionName,
                           boost::int32_t const i_LineNo );

  static void SetWriteTime( bool i_WriteTime ) { ms_WriteTime = i_WriteTime; }

  static LogLevel GetVerbosity() { return ms_Verbosity; }
  /// set the logging verbosity, all messages with level <= verbosity are logged
  static void SetVerbosity(LogLevel i_Verbosity) { ms_Verbosity = i_Verbosity; }

  static void AddLoggingDestination(LogLevel i_Level, LogOstream *i_LoggingDest) 
  {
    ms_LoggingDestinations[i_Level].push_back(i_LoggingDest); 
  }
  static void AddLoggingDestination(LogLevel i_Level, std::ofstream *i_LoggingDest) 
  {
    ms_LoggingDestinations[i_Level].push_back(reinterpret_cast<LogOstream *>(i_LoggingDest)); 
  }

  /**
   * Init Logging with defaults
   *  - a logfile
   *  - log err&fatal to logerr, all other things to loginf
   */
  static void InitDefaults(std::ofstream *i_LogFile = NULL, bool i_WriteTime = false);

protected:
  std::ostringstream m_OS;

  static bool ms_WriteTime;
  static LogLevel ms_Verbosity;
  static std::map<LogLevel, std::vector<LogOstream *> > ms_LoggingDestinations;

private:
  Log(const Log&);
  Log& operator=(const Log&);

  LogLevel m_MessageLevel;

  void WritePrefix(char const * i_pFilePath, char const * i_pFunctionName, boost::int32_t const i_LineNo);
};

// convenience macros
#define LOG(level)                        \
  if( level <= Log::GetVerbosity() )      \
    Log().Get(level, __FILE__, __FUNCTION__, __LINE__) 

#define logdet    LOG(LogLevel::Detail)
#define loginf    LOG(LogLevel::Info)
#define logwarn   LOG(LogLevel::Warning)
#define logerr    LOG(LogLevel::Error)
#define logfat    LOG(LogLevel::Fatal)

#include "LogUtilFunc.h"

#endif // __LOGGER_H__
