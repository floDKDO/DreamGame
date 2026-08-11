#pragma once

#include "vertex.h"

#include <vector>

class Vertices
{
	public:
		explicit Vertices(uint64_t number);
		void add_position_attributes(std::vector<glm::vec3> position_vector);
		void add_normal_attributes(std::vector<glm::vec3> normal_vector);
		void add_texture_coordinates_attributes(std::vector<glm::vec2> texture_coordinates_vector);
		void add_color_attributes(std::vector<glm::vec4> color_vector);
		std::size_t get_vertices_number() const;
		void print() const;

		bool has_position_attribute() const;
		bool has_normal_attribute() const;
		bool has_texture_coordinates_attribute() const;
		bool has_color_attribute() const;
		
	private: 
		enum AttributeByteValue : uint8_t
		{
			POSITION_BYTE = 1,
			NORMAL_BYTE = 2,
			TEXTURE_COORD_BYTE = 4,
			COLOR_BYTE = 8
		};

		std::vector<Vertex> vertices_;
		uint8_t attributes_byte_;

	public:
		const Vertex* get_vertices_data() const;
};