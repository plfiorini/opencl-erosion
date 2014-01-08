#include <gfx/include/Material_factory.h>
#include <gfx/include/Colored_material.h>

#include <common/include/Tools.h>

namespace mkay
{
  Abstract_material * Material_factory::create(Material_type i_type)
  {
    switch(i_type)
    {
      case Material_type::Colored: return new Colored_material();
//       case Material_type::Textured: return new Textured_material();
      default: 
        BOOST_THROW_EXCEPTION(
          Material_exception()
            << errinfo_cstr("Material type not registered!")
            << errinfo_int32_t(to_integral(i_type))
        );
    }
  }
} // namespace mkay