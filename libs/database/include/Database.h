#pragma once
#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <common/include/Exception.h>

struct sqlite3;

namespace mkay
{
  class Database
  {
  public:
    Database(std::string const & i_filename)
    : m_filename(i_filename)
    , m_database(NULL)
    {
    }
    ~Database();
    
    void open();
    void save();
    void close();
    
  private:
    std::string m_filename;
    sqlite3 *m_database;
  };
  
  /// define a new exception type
  class Database_exception 
    : public Exception
  {
  };
} // namespace mkay

#endif // __DATABASE_H__