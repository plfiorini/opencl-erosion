// vertex shader variables
SHADER_VAR(uniform, mat4, model_view_projection);
SHADER_VAR(in, vec4, in_position);
// fragment shader variables
SHADER_VAR(uniform, samplerCube, tex_cube);
SHADER_VAR(out, vec4, out_frag_color);