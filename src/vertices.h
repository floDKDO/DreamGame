#include <GL/glew.h>
#include <vector>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

//TODO : public/private et mettre dans un fichier à part ??
class Vertices
{
	public:
		//TODO : public/private et mettre dans un fichier à part ??
		enum AttributeIndex : GLuint
		{
			Position = 0,
			Normal = 1,
			TextureCoord = 2,
			Color = 3
		};

		struct Vertex
		{
			Vertex()
				: position_(glm::vec3(0.0f, 0.0f, 0.0f)), normal_(glm::vec3(0.0f, 0.0f, 0.0f)), texture_coordinates_(glm::vec2(0.0f, 0.0f)), color_(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))
			{}

			Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texture_coordinates, glm::vec4 color)
				: position_(position), normal_(normal), texture_coordinates_(texture_coordinates), color_(color)
			{}

			glm::vec3 position_;			//0
			glm::vec3 normal_;				//1
			glm::vec2 texture_coordinates_; //2
			glm::vec4 color_;				//3
		};

		Vertices(uint64_t number);
		void add_position_attributes(std::vector<glm::vec3> position_vector);
		void add_normal_attributes(std::vector<glm::vec3> normal_vector);
		void add_texture_coordinates_attributes(std::vector<glm::vec2> texture_coordinates_vector);
		void add_color_attributes(std::vector<glm::vec4> color_vector);
		std::size_t get_vertices_number() const;
		std::size_t get_attribute_offset(AttributeIndex attribute_index) const;

		std::vector<Vertex> vertices_;
		bool has_positions_;
		bool has_normals_;
		bool has_texture_coordinates_;
		bool has_colors_;
};