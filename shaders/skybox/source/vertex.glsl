#version 430

// include common shader vars
#include <common_shader/include/vars.hgl>
#include <skybox/include/vars.hgl>

// output vars of this stage
out vec3 vtx_tex_coords;

void main(void) 
{
  gl_Position = model_view_projection * in_position;

  vtx_tex_coords = normalize(in_position.xyz);
}
