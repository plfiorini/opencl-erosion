#version 430

#include <common_shader/include/vars.hgl>

out vec3 vtx_vertex_world;
out vec3 vtx_normal;
out vec3 vtx_light_dir;
out vec3 vtx_view_dir;

void main(void) 
{
  vec4 pos = vec4(in_position.xyz, 1.0);

  gl_Position = model_view_projection * pos;

  // calculate vertex world position    
  vtx_vertex_world = vec3(model_matrix * pos);  
  // transform normal
  vtx_normal = normalize(vec3(model_matrix * in_normal));
  // calculate light vector from light world position - vertex world position  
  vtx_light_dir = normalize(light_position - vtx_vertex_world);
  // calculate view vector from cam world position - vertex world position
  vtx_view_dir = normalize(vtx_vertex_world - camera_position);       
}
