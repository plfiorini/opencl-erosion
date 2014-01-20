project(gfx)

include(build_functions)

add_standard_lib(
  common
)

use_opengl()
use_devil()
use_third_party(libsdl2)
use_third_party(libsdl2_ttf)
use_boost(filesystem)

use_shader(
  common_shader
  skybox_shader
  color_shader
)