#ifndef __MKAY_OBJECT_FACTORY_H__
#define __MKAY_OBJECT_FACTORY_H__

#include <string>
#include <map>
#include <utility>
#include <iostream>

#include <boost/exception/all.hpp>
#include <boost/shared_ptr.hpp>

namespace registry
{
  /// \brief create function prototype
  template <typename ObjectType>
  std::shared_ptr<ObjectType> create(std::string const & i_name);

  /// \brief error info typedef
  typedef boost::error_info<struct tag_registry_info, std::string> Registry_info;
  
  /// \brief struct for registry exception
  struct Registry_error : virtual boost::exception, virtual std::exception {};
} // namespace registry

/// \brief simple macro to register object types for use in the object factory
/// \param SUBTYPE the specialiced type
/// \param BASETYPE the base class of SUBTYPE
#define REGISTER_TYPE(SUBTYPE, BASETYPE)                                      \
  namespace registry                                                          \
  {                                                                           \
    namespace detail                                                          \
    {                                                                         \
      namespace                                                               \
      {                                                                       \
        template<typename ObjectSubType, typename ObjectType>                 \
        class Object_registration;                                            \
                                                                              \
        template<>                                                            \
        class Object_registration<SUBTYPE, BASETYPE>                          \
        {                                                                     \
            static const ::registry::detail::Registry_entry<SUBTYPE, BASETYPE>& reg;       \
        };                                                                    \
                                                                              \
        const ::registry::detail::Registry_entry<SUBTYPE, BASETYPE>&          \
            Object_registration<SUBTYPE, BASETYPE>::reg =                     \
                ::registry::detail::Registry_entry<SUBTYPE, BASETYPE>::Instance(SUBTYPE::get_type_name());     \
      }                                                                       \
    }                                                                         \
  }
  
#define GET_FACTORY(BASETYPE)                                                 \
  registry::detail::get_object_registry<BASETYPE>()

namespace registry
{
  /// \brief internal implementation
  namespace detail
  {
    /// \brief template struct to define various types
    template <typename ObjectType>
    struct Def
    {
      typedef ObjectType* (*Create_object_func)();
      typedef std::map<std::string, Create_object_func> Object_registry;
    };

    /// \brief template function to obtain the object registry for type ObjectType
    template <typename ObjectType>
    inline typename Def<ObjectType>::Object_registry& get_object_registry()
    {
      static typename Def<ObjectType>::Object_registry reg;
      return reg;
    }

    /// \brief template function to create an instance of type ObjectSubType
    /// \return created instance
    template <typename ObjectSubType, typename ObjectType>
    ObjectType* create_object()
    {
      return new ObjectSubType();
    }
    
    /// \brief handles object registration
    template <typename ObjectSubType, typename ObjectType>
    class Registry_entry
    {
    public:
      /// \brief static function to obtain the instance of Registry_entry for the
      ///        specified type & subtype
      /// \return reference to the registry entry
      static 
      Registry_entry<ObjectSubType, ObjectType>& Instance(std::string const & i_name)
      {
        static Registry_entry<ObjectSubType, ObjectType> inst(i_name);
        return inst;
      }
      
    private:
      /// \brief private constructor to register the subtype at the object registry
      Registry_entry(std::string const & i_name)
      {
        typename Def<ObjectType>::Object_registry & reg = get_object_registry<ObjectType>();
        typename Def<ObjectType>::Create_object_func func = create_object<ObjectSubType, ObjectType>;
        
        // insert into map
        std::pair<typename Def<ObjectType>::Object_registry::iterator, bool> ret =
          reg.insert(typename Def<ObjectType>::Object_registry::value_type(i_name, func));
          
        // if insertion wasn't successful
        if (ret.second == false)
        {
          BOOST_THROW_EXCEPTION(
            Registry_error() << Registry_info("object type already registered: " + i_name)
          );
        }
      }
      
      Registry_entry(Registry_entry<ObjectSubType, ObjectType> const &);
      Registry_entry & operator=(Registry_entry<ObjectSubType, ObjectType> const &);
    };   
  }

  /// \brief factory create function
  template <typename ObjectType>
  std::shared_ptr<ObjectType> create(std::string const & i_name)
  {
    typename detail::Def<ObjectType>::Object_registry& reg = detail::get_object_registry<ObjectType>();
    typename detail::Def<ObjectType>::Object_registry::iterator it = reg.find(i_name);
    
    if ( it == reg.end() )
    {
      BOOST_THROW_EXCEPTION(
        Registry_error() << Registry_info("object type not found: " + i_name)
      );
    }
    
    typename detail::Def<ObjectType>::Create_object_func func = it->second;
    std::shared_ptr<ObjectType> temp_ptr( func() );
    return temp_ptr;
  }

} // namespace registry

/// \brief operator to print registry content
/// \remarks unusable - because does not match anyting ... 
template <typename ObjectType>
std::ostream & operator <<( 
  std::ostream & ostr,
  const typename registry::detail::Def<ObjectType>::Object_registry & i_registry )
{
  ostr << "registered object types:" << std::endl;
  for ( auto it = i_registry.begin(); it != i_registry.end(); ++it )
  {
    ostr << " * " << it->first << std::endl;
  }
  return ostr;
}

#endif // __MKAY_OBJECT_FACTORY_H__
