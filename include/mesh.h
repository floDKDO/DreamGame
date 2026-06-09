#include "shader_program.h"

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

class Mesh
{
	public:
		//TODO : public/private et mettre dans un fichier à part ??
		struct TextureInfo
		{
			GLuint texture_index_;
			std::string image_path_;
			GLint mag_filter_;
			GLint min_filter_;
			GLint wrap_s_;
			GLint wrap_t_;
		};

		struct Texture
		{
			GLuint texture_id_;
			TextureInfo texture_info_;
		};


		//TODO : public/private et mettre dans un fichier à part ??
		struct Vertex
		{
			Vertex()
				: position_(glm::vec3(0.0f, 0.0f, 0.0f)), normal_(glm::vec3(0.0f, 0.0f, 0.0f)), texture_coordinates_(glm::vec2(0.0f, 0.0f)), color_(glm::vec3(0.0f, 0.0f, 0.0f))
			{}

			Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texture_coordinates, glm::vec3 color)
				: position_(position), normal_(normal), texture_coordinates_(texture_coordinates), color_(color)
			{}

			glm::vec3 position_;			//0
			glm::vec3 normal_;				//1
			glm::vec2 texture_coordinates_; //2
			glm::vec3 color_;				//3
		};

		//TODO : public/private et mettre dans un fichier à part ??
		struct Vertices
		{
			Vertices(uint64_t number)
				: vertices_(number), has_positions_(false), has_colors_(false), has_texture_coordinates_(false), has_normals_(false)
			{}

			void add_position_attributes(std::vector<glm::vec3> position_vector)
			{
				for(std::size_t i = 0; i < vertices_.size(); ++i)
				{
					vertices_[i].position_ = position_vector[i];
				}
				has_positions_ = true;
			}

			void add_normal_attributes(std::vector<glm::vec3> normal_vector)
			{
				for(std::size_t i = 0; i < vertices_.size(); ++i)
				{
					vertices_[i].normal_ = normal_vector[i];
				}
				has_normals_ = true;
			}

			void add_texture_coordinates_attributes(std::vector<glm::vec2> texture_coordinates_vector)
			{
				for(std::size_t i = 0; i < vertices_.size(); ++i)
				{
					vertices_[i].texture_coordinates_ = texture_coordinates_vector[i];
				}
				has_texture_coordinates_ = true;
			}

			void add_color_attributes(std::vector<glm::vec3> color_vector)
			{
				for(std::size_t i = 0; i < vertices_.size(); ++i)
				{
					vertices_[i].color_ = color_vector[i];
				}
				has_colors_ = true;
			}

			std::size_t get_vertices_number() const
			{
				return vertices_.size();
			}

			std::vector<Vertex> vertices_;
			bool has_positions_;
			bool has_normals_;
			bool has_texture_coordinates_;
			bool has_colors_;
		};

		Mesh(std::vector<GLushort> ebo_values, Vertices vertices, std::vector<TextureInfo> textures_info, GLenum draw_mode);
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
		void create_textures();
		void load_mesh();

		std::vector<GLushort> ebo_values_;
		Vertices vertices_;
		std::vector<Texture> textures_;
		GLuint ebo_, vbo_, vao_;
		GLenum draw_mode_;
};