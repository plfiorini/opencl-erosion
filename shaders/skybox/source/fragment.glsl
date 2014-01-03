#version 430

// include common shader vars
#include <skybox/include/vars.h>

// input vars from vertex stage
in vec3 vtx_tex_coords;

void main(void) 
{ 
  out_frag_color = texture(tex_cube, vtx_tex_coords);
}
