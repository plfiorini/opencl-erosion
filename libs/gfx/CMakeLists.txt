project(gfx)
reset_project()

include(build_functions)

list(APPEND LIBS common)

add_standard_lib(
  ${LIBS}  
)

use_opengl()
use_devil()
use_sdl()
use_sdlttf()
use_boost(filesystem)

use_shader(
  common_shader
  skybox_shader
  color_shader
)