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
  
  /// typedef for error information size_t integer
  typedef boost::error_info<struct tag_error_size_t, size_t> errinfo_size_t; 
  
  /// typedef for error information 32bit integer
  typedef boost::error_info<struct tag_error_int32_t, int32_t> errinfo_int32_t; 
  
  class Mkay_exception
    : public virtual boost::exception
    , public virtual std::exception
  {
  public:
    Mkay_exception() = default;
  };
} // namespace mkay

#endif // __MKAY_EXCEPTION_H__