#version 430

#include <common_shader/include/vars.hgl>

//uniform vec4 materialColor;

// input vars
in vec3 vtx_vertex_world;
in vec3 vtx_normal;
in vec3 vtx_light_dir;
in vec3 vtx_view_dir;

void main(void) 
{
  float f_phong_coeff = 4.0;
  vec4 v4_material_color = vec4(1.0, 0.0, 0.0, 1.0);

  // attenuation calculation
  float f_dist = distance(light_position, vtx_vertex_world);
  float f_atten_factor = (1.0 + light_range)/
    (1.0 + light_constant_attenuation + 
     light_linear_attenuation * f_dist + 
     light_quadratic_attenuation * f_dist * f_dist);

  vec3 v3_normal = normalize(vtx_normal);
  vec3 v3_light_dir = normalize(vtx_light_dir);
  vec3 v3_view_dir = normalize(vtx_view_dir);

  // diffuse lighting
  float f_diffuse = dot(v3_normal, v3_light_dir);

  // specular lighting
  vec3 v3_reflected = normalize( reflect( -v3_light_dir, v3_normal) ); 
  float f_specular = clamp( dot(v3_reflected, v3_view_dir), 0.0, 1.0 );

  // self shadowing term  
  float f_shadow = clamp( 4.0*f_diffuse, 0.0, 1.0 );

  vec4 IAmbient  = light_ambient_color * v4_material_color;
  vec4 IDiffuse  = light_diffuse_color * f_diffuse * v4_material_color;
  vec4 ISpecular = light_specular_color * pow(f_specular, f_phong_coeff) * v4_material_color;

  //out_frag_color = IAmbient + f_shadow * f_atten_factor * (IDiffuse + ISpecular);
  out_frag_color = vec4(1.0, 0.0, 0.0, 1.0);
}
