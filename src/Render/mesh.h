#pragma once

#include "vertices.h"

class Mesh
{
	public: 
		Mesh(std::vector<GLushort> ebo_values, Vertices vertices, std::vector<std::string> texture_keys, GLenum draw_mode);
		Mesh(std::vector<GLushort> ebo_values, Vertices vertices, GLenum draw_mode);

		void draw() const;
		void destroy();

	private:
		void load_vertex_attribute(GLuint vbo_binding_index, attribute::Name attribute_name);
		void create_ebo();
		void create_vbo();
		void create_vao();
		void destroy_all_buffers() const;
		void create_textures();
		void load_mesh();

		std::vector<GLushort> ebo_values_;
		Vertices vertices_;
		std::vector<std::string> texture_keys_;
		GLuint ebo_, vbo_, vao_;
		GLenum draw_mode_;
};