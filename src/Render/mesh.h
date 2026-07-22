#pragma once

#include "shader_program.h"
#include "texture.h"
#include "vertices.h"

#include <glm/mat4x4.hpp>

class Mesh
{
	public: 
		Mesh(std::vector<GLushort> ebo_values, Vertices vertices, std::vector<Texture> textures, GLenum draw_mode);
		void draw(ShaderProgram& shader_progam);

	private:
		void load_vertex_attribute(GLuint vbo_binding_index, Vertices::AttributeIndex attribute_index);
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