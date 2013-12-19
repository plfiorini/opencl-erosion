#pragma once
#ifndef __ENUM_GENERATOR_H__
#define __ENUM_GENERATOR_H__

#include <iostream>
#include <string>
#include <vector>

#include <boost/preprocessor.hpp>
#include <boost/cstdint.hpp>
#include <boost/exception/all.hpp>
#include <boost/exception/exception.hpp>
#include <boost/exception/error_info.hpp>
#include <boost/exception/info.hpp>
#include <boost/algorithm/string.hpp>

#include <common/include/Mkay_exception.h>

/**
 * Usage:
 * 
 * >> Header File
 * 
 *  #define Enum_definition \
 *    (Element_1)(Element_2)...
 * 
 *  DEF_ENUM_HEADER( Enum_name, Enum_definition );
 * 
 * 
 * >> Implementation File
 * 
 *  DEF_ENUM_DATA( Enum_name, Enum_definition );
 * 
 */

namespace mkay
{
  class Enum_exception
    : public Mkay_exception
  {
  };
  
#define DEF_ENUM_LINE_HEADER(z, I, _seq) \
  BOOST_PP_SEQ_ELEM(I, _seq),

#define DEF_ENUM_HEADER(_name, _seq)  \
  namespace _name \
  { \
    extern char const * const to_string[]; \
    enum ENUM\
    {\
      BOOST_PP_REPEAT(BOOST_PP_SEQ_SIZE(_seq), DEF_ENUM_LINE_HEADER, _seq)  \
      ENUM_MAX\
    };\
    char const * const checked_to_string(ENUM i_enum); \
    ENUM checked_to_enum(const char * i_name); \
  }; \
  std::ostream & operator<< (std::ostream& o_stream, _name::ENUM const i_enum);

#define DEF_ENUM_LINE_DATA(z, I, _seq) \
  BOOST_PP_STRINGIZE(BOOST_PP_SEQ_ELEM(I, _seq)),
  
#define DEF_ENUM_DATA(_name, _seq) \
  namespace _name \
  { \
    char const * const to_string[] = { \
      BOOST_PP_REPEAT(BOOST_PP_SEQ_SIZE(_seq), DEF_ENUM_LINE_DATA, _seq)  \
    }; \
    char const * const checked_to_string(_name::ENUM i_enum) \
    { \
      if ( i_enum < 0 || i_enum >= ENUM_MAX ) \
      { \
        BOOST_THROW_EXCEPTION( Enum_exception{} \
          << mkay::errinfo_str{"out of bounds!"} \
          << mkay::errinfo_size_t{i_enum} \
        ); \
      } \
      return to_string[i_enum]; \
    } \
    ENUM checked_to_enum(const char * i_name) \
    { \
      for ( int i=0; i<ENUM_MAX; ++i ) \
      { \
        if ( boost::equals(to_string[i], i_name) ) \
        { \
          return (ENUM)i; \
        } \
      } \
      BOOST_THROW_EXCEPTION( Enum_exception{} \
        << mkay::errinfo_str(std::string{"unknown enum: '"} + i_name + "'") \
      ); \
    } \
  } \
  \
  std::ostream & operator<< (std::ostream& o_stream, _name::ENUM const i_enum) \
  { \
    o_stream << _name::to_string[i_enum]; \
    return o_stream; \
  }

#define SWITCH( _name, _testVal ) \
  { _name::ENUM const ___TTT___ = _testVal; if(true==false) {
#define CASE( _caseVal ) \
  } else if( ___TTT___ == _caseVal ) {
#define ESWITCH } }

} // namespace mkay

#endif // __ENUM_GENERATOR_H__