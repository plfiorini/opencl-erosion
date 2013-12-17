#pragma once
#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <boost/exception/all.hpp>

namespace mkay
{
  /// typedef for error information message
  typedef boost::error_info<struct tag_error_cstr, std::string> errinfo_cstr;
  
  /// typedef for error information message
  typedef boost::error_info<struct tag_error_msg, std::string> errinfo_str;
  
  class Exception
    : public boost::exception
  {
  public:
    Exception() = default;
  };
} // namespace mkay

#endif // __EXCEPTION_H__