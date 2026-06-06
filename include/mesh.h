#include "shader_program.h"

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

class Mesh
{
	public:
		struct Vertex
		{
			Vertex(glm::vec3 position, glm::vec3 color, glm::vec2 texture_coordinates)
				: position_(position), color_(color), texture_coordinates_(texture_coordinates)
			{}

			glm::vec3 position_;
			glm::vec3 color_;
			glm::vec2 texture_coordinates_;
		};

		Mesh(std::vector<GLushort> ebo_values, std::vector<Vertex> attribute_values, GLenum draw_mode);
		void draw(ShaderProgram& shader_progam);

	private:
		void create_ebo();
		void create_vbo();
		void create_vao();
		void load_mesh();

		std::vector<GLushort> ebo_values_;
		std::vector<Vertex> attribute_values_;
		GLuint ebo_, vbo_, vao_;
		GLenum draw_mode_;
};