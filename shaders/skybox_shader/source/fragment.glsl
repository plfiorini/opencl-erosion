#version 430

// include common shader vars
#include <common_shader/include/vars.hgl>
#include <skybox_shader/include/vars.hgl>

// input vars from vertex stage
in vec3 vtx_tex_coords;

void main(void) 
{ 
  out_frag_color = texture(tex_cube, vtx_tex_coords);
}
