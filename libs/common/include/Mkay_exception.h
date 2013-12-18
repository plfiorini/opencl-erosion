#pragma once
#ifndef __MKAY_EXCEPTION_H__
#define __MKAY_EXCEPTION_H__

#include <boost/exception/all.hpp>

namespace mkay
{
  /// typedef for error information message
  typedef boost::error_info<struct tag_error_cstr, std::string> errinfo_cstr;
  
  /// typedef for error information message
  typedef boost::error_info<struct tag_error_msg, std::string> errinfo_str;
  
  class Mkay_exception
    : public boost::exception
    , public virtual std::exception
  {
  public:
    Mkay_exception() = default;
  };
} // namespace mkay

#endif // __MKAY_EXCEPTION_H__