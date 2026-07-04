#pragma once

#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>

class Vertices
{
	public:
		enum AttributeIndex : GLuint
		{
			POSITION = 0,
			NORMAL = 1,
			TEXTURE_COORD = 2,
			COLOR = 3
		};

		explicit Vertices(uint64_t number);
		void add_position_attributes(std::vector<glm::vec3> position_vector);
		void add_normal_attributes(std::vector<glm::vec3> normal_vector);
		void add_texture_coordinates_attributes(std::vector<glm::vec2> texture_coordinates_vector);
		void add_color_attributes(std::vector<glm::vec4> color_vector);
		std::size_t get_vertices_number() const;
		std::size_t get_attribute_offset(AttributeIndex attribute_index) const;
		GLint get_number_of_attribute_values(AttributeIndex attribute_index) const;
		size_t get_vertex_size() const;
		size_t get_length() const;
		void print() const;

		bool has_positions_;
		bool has_normals_;
		bool has_texture_coordinates_;
		bool has_colors_;

	private: 
		struct Vertex
		{
			Vertex()
				: position_(glm::vec3(0.0f, 0.0f, 0.0f)), normal_(glm::vec3(0.0f, 0.0f, 0.0f)), texture_coordinates_(glm::vec2(0.0f, 0.0f)), color_(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))
			{}

			Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texture_coordinates, glm::vec4 color)
				: position_(position), normal_(normal), texture_coordinates_(texture_coordinates), color_(color)
			{}

			glm::vec3 position_;			//attribute_index = AttributeIndex::Position
			glm::vec3 normal_;				//attribute_index = AttributeIndex::Normal
			glm::vec2 texture_coordinates_; //attribute_index = AttributeIndex::TextureCoord
			glm::vec4 color_;				//attribute_index = AttributeIndex::Color
		};

		std::vector<Vertex> vertices_;

	public:
		const Vertex* get_data() const;	
};