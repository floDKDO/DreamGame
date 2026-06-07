#include "shader_program.h"

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

class Mesh
{
	public:
		struct Vertex
		{
			Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texture_coordinates, glm::vec3 color)
				: position_(position), normal_(normal), texture_coordinates_(texture_coordinates), color_(color)
			{}

			glm::vec3 position_;			//0
			glm::vec3 normal_;				//1
			glm::vec2 texture_coordinates_; //2
			glm::vec3 color_;				//3
		};

		struct Vertices
		{
			Vertices()
				: has_positions_(false), has_colors_(false), has_texture_coordinates_(false), has_normals_(false)
			{}

			std::vector<Vertex> vertices_;
			bool has_positions_;
			bool has_colors_;
			bool has_texture_coordinates_;
			bool has_normals_;
		};

		Mesh(std::vector<GLushort> ebo_values, Vertices vertices, GLenum draw_mode);
		void draw(ShaderProgram& shader_progam);

	private:
		enum AttributeIndex : GLuint
		{
			Position = 0,
			Normal = 1,
			TextureCoord = 2,
			Color = 3
		};

		void load_vertex_attribute(GLuint vbo_binding_index, AttributeIndex attribute_index, GLint size);
		void create_ebo();
		void create_vbo();
		void create_vao();
		void load_mesh();

		std::vector<GLushort> ebo_values_;
		Vertices vertices_;
		GLuint ebo_, vbo_, vao_;
		GLenum draw_mode_;
};