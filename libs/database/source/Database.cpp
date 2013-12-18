#include <database/include/Database.h>

#include <common/include/Logger.h>

#include <sqlite3.h>

namespace mkay
{
  Database::~Database()
  {
    if (m_database)
    {
      this->close();
    }
  }
  
  void Database::open()
  {
    if ( SQLITE_OK != sqlite3_open(m_filename.c_str(), &m_database) )
    {
      const char * msg = sqlite3_errmsg(m_database);
      m_database = nullptr;
      throw Database_exception() 
        << errinfo_str("could not open database: "+m_filename)
        << errinfo_cstr(msg);
    }
    else
    {
      logdeb << "database " << m_filename << " opened successfully ..." << std::endl;
    }
  }
  
  void Database::save()
  {
    if (m_database)
    {
      this->close();
      this->open();
    }
    else
    {
      throw Database_exception()
        << errinfo_str("database connection not yet opened");
    }
  }
  
  void Database::close()
  {
    if ( SQLITE_OK != sqlite3_close(m_database) )
    {
      logerr << "could not close database: " << m_filename << std::endl;
    }
    else
    {
      logdeb << "closed database: " << m_filename << std::endl;
    }
    m_database = nullptr;
  }
} // namespace mkay