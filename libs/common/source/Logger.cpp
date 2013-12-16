#include <common/include/Logger.h>

#include <time.h>
#include <cstring>

namespace mkay
{
    
  //------------------------------------------------------------------------------
  // STATIC
  bool Logger::ms_write_time_prefix = false;
  Log_level Logger::ms_verbosity;
  std::map<Log_level, std::vector<Log_ostream *> > Logger::ms_logging_destinations;

  //------------------------------------------------------------------------------
  // STATIC
  void Logger::init_defaults(std::ofstream *i_log_file /* = NULL */, bool i_write_time_prefix /* = false */)
  {
    set_verbosity(Log_level::Info);
    set_write_time_prefix(i_write_time_prefix);

    for( int8_t i=0; 
         i<=static_cast<int8_t>(Log_level::Error); 
         ++i )
    {
      add_logging_destination(static_cast<Log_level>(i), &std::cerr);
      if (i_log_file != NULL)
      {
        add_logging_destination(static_cast<Log_level>(i), i_log_file);
      }
    }
    for( int8_t i=static_cast<int8_t>(Log_level::Warning); 
         i<static_cast<int8_t>(Log_level::Log_level_count); 
         ++i )
    {
      add_logging_destination(static_cast<Log_level>(i), &std::cout);
      if (i_log_file != NULL)
      {
        add_logging_destination(static_cast<Log_level>(i), i_log_file);
      }
    }
  }

  //------------------------------------------------------------------------------
  Logger::~Logger()
  {
    if( m_message_level <= ms_verbosity )
    {
      std::vector<std::ostream *> &levels_log_dests = ms_logging_destinations[m_message_level];

      for(std::size_t i=0; i<levels_log_dests.size(); ++i)
      {
        *levels_log_dests[i] << m_output_stream.str();
        levels_log_dests[i]->flush();
      }
    }
  }

  //------------------------------------------------------------------------------
  std::ostringstream & Logger::get( Log_level i_level
                                  , char const * i_file_path
                                  , char const * i_function_name
                                  , int32_t const i_line_no )
  {
    m_message_level = i_level;
    this->write_prefix(i_file_path, i_function_name, i_line_no);
    return this->m_output_stream;
  }

  //------------------------------------------------------------------------------
  void Logger::write_prefix( char const * i_file_path
                          , char const * i_function_name
                          , int32_t const i_line_no )
  {
    if( ms_write_time_prefix )
    {
      // get the current time & date
      time_t currentTime;
      time( &currentTime );
      char *timeString = ctime( &currentTime );
      // delete the newline at the end
      timeString[strlen(timeString)-1] = '\0';

      m_output_stream << timeString << " | ";
    }

    switch(m_message_level)
    {
      case Log_level::Fatal:    m_output_stream << "FATAL "; break;
      case Log_level::Error:    m_output_stream << "ERROR "; break;
      case Log_level::Warning:  m_output_stream << "WARN  "; break;
      case Log_level::Info:     m_output_stream << "INFO  "; break;
      case Log_level::Detail:   m_output_stream << "DETAIL"; break;
      default:                  m_output_stream << "<UNKNWN>"; break;
    }

    // get filename from path
    if( i_file_path )
    {
  #ifdef WIN32
      char *file_name = strrchr(i_file_path, '\\');
      file_name++;
  #else
      char const *file_name = strrchr(i_file_path, '/');
      if ( !file_name )
      {
        file_name = i_file_path;
      }
      else
      {
        file_name++;
      }
  #endif
      m_output_stream << " | " << file_name;
    }

    if (i_function_name)
    {
      m_output_stream << " | " << i_function_name;
    }


    m_output_stream << ":" << i_line_no << " |> ";
  }

} // namespace mkay