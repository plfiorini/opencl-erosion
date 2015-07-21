#include <gfx/include/Skybox.h>
#include <gfx/include/Camera.h>
#include <gfx/include/Ressource_manager.h>
#include <common/include/Tools.h>

#include <common_shader/include/vars.hgl>
#include <skybox_shader/include/vars.hgl>

using namespace std;

namespace mkay
{
  void Skybox::init()
  {
    m_shader = RESMAN.get<Shader_program>("skybox_shader");
    m_texture = RESMAN.get<Texture_cube_map>("cubemap_stormy_day");

		buffer_data();
  }
  
	void Skybox::buffer_data()
	{
		glGenBuffers(to_integral(VBO_skybox_type::Count), m_vbo_id);

		const GLsizei vertices = 24;
		m_triangle_count = 36;

		const GLfloat size = -1.0f;

		// the 4 vertices of a box 
		const GLfloat box[vertices][4] =
		{
			/* front */
			{ -size, -size, size, 1.0f },
			{ size, -size, size, 1.0f },
			{ size, size, size, 1.0f },
			{ -size, size, size, 1.0f },

			/* left */
			{ -size, -size, -size, 1.0f },
			{ -size, -size, size, 1.0f },
			{ -size, size, size, 1.0f },
			{ -size, size, -size, 1.0f },

			/* right */
			{ size, -size, size, 1.0f },
			{ size, -size, -size, 1.0f },
			{ size, size, -size, 1.0f },
			{ size, size, size, 1.0f },

			/* top */
			{ -size, size, size, 1.0f },
			{ size, size, size, 1.0f },
			{ size, size, -size, 1.0f },
			{ -size, size, -size, 1.0f },

			/* bottom */
			{ -size, -size, -size, 1.0f },
			{ size, -size, -size, 1.0f },
			{ size, -size, size, 1.0f },
			{ -size, -size, size, 1.0f },

			/* back */
			{ size, -size, -size, 1.0f },
			{ -size, -size, -size, 1.0f },
			{ -size, size, -size, 1.0f },
			{ size, size, -size, 1.0f },
		};

		const GLuint boxindices[36] =
		{
			/* front */
			0, 1, 2,
			2, 3, 0,

			/* left */
			4, 5, 6,
			6, 7, 4,

			/* right */
			8, 9, 10,
			10, 11, 8,

			/* top */
			12, 13, 14,
			14, 15, 12,

			/* bottom */
			16, 17, 18,
			18, 19, 16,

			/* back */
			20, 21, 22,
			22, 23, 20,
		};

	
		// buffer all data

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id[to_integral(VBO_skybox_type::Position)]);
		glBufferData(GL_ARRAY_BUFFER, vertices * 4 * sizeof(GLfloat), box, GL_STATIC_DRAW);
		GLint vertex_attrib_location = m_shader->get_attribute_location(in_position);
		glEnableVertexAttribArray(vertex_attrib_location);
		glVertexAttribPointer(vertex_attrib_location, 4, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo_id[to_integral(VBO_skybox_type::Index)]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_triangle_count * sizeof(GL_UNSIGNED_INT), boxindices, GL_STATIC_DRAW);

		// disable pointers & unbind buffers
		glDisableVertexAttribArray(vertex_attrib_location);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void Skybox::render(Camera & i_camera)
  {
    // setup camera without translation
    i_camera.setup_scene(true);
       
    m_shader->use();
      
    m_texture->set(m_shader, GL_TEXTURE0, tex_cube);
    
    glDisable(GL_DEPTH_TEST);
    //glDisable(GL_BLEND);
    
		draw(i_camera);

    m_texture->unset();
    m_shader->disable();
  }
	
	void Skybox::draw(Camera &i_camera)
	{
		glm::mat4 const MVP = i_camera.get_projection_view_matrix();

		glUniformMatrix4fv(m_shader->get_uniform_location(model_view_projection), 1, GL_FALSE, glm::value_ptr(MVP));

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id[to_integral(VBO_skybox_type::Position)]);
		GLint vertex_attrib_location = m_shader->get_attribute_location(in_position);
		glEnableVertexAttribArray(vertex_attrib_location);
		glVertexAttribPointer(vertex_attrib_location, 4, GL_FLOAT, GL_FALSE, 0, 0);

		// Bind our third VBO as being the active buffer and storing vertex array indicies
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo_id[to_integral(VBO_skybox_type::Index)]);

		glDrawElements(GL_TRIANGLES, m_triangle_count, GL_UNSIGNED_INT, 0);

		// unbind & disable
		glDisableVertexAttribArray(vertex_attrib_location);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

} // namespace mkay

