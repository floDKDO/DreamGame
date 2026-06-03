#include "shader_program.h"

#include <glm/vec3.hpp>

class Mesh
{
	public:
		Mesh(std::vector<GLushort> ebo_values, std::vector<glm::vec3> position_attribute_values, GLenum draw_mode);
		void draw(ShaderProgram& shader_progam);

	private:
		void create_ebo();
		void create_vbo();
		void create_vao();
		void load_mesh();

		std::vector<GLushort> ebo_values_;
		std::vector<glm::vec3> position_attribute_values_;
		GLuint ebo_, vbo_, vao_;
		GLenum draw_mode_;
};