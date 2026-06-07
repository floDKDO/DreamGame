#include "mesh.h"

#include <iostream>

Mesh::Mesh(std::vector<GLushort> ebo_values, Vertices vertices, GLenum draw_mode)
	: ebo_values_(ebo_values), vertices_(vertices), draw_mode_(draw_mode == -1 ? GL_TRIANGLES : draw_mode)
{
	load_mesh();
}

void Mesh::load_vertex_attribute(GLuint vbo_binding_index, AttributeIndex attribute_index, GLint size)
{
	std::size_t offset = 0;
	switch(attribute_index)
	{
		case AttributeIndex::Position:
			offset = offsetof(Vertex, position_);
			break;

		case AttributeIndex::Normal:
			offset = offsetof(Vertex, normal_);
			break;

		case AttributeIndex::TextureCoord:
			offset = offsetof(Vertex, texture_coordinates_);
			break;

		case AttributeIndex::Color:
			offset = offsetof(Vertex, color_);
			break;

		default:
			std::cout << "****ERROR****\n";
			break;
	}
	glEnableVertexArrayAttrib(vao_, attribute_index);
	glVertexArrayAttribFormat(vao_, attribute_index, size, GL_FLOAT, GL_FALSE, GLuint(offset));
	glVertexArrayAttribBinding(vao_, attribute_index, vbo_binding_index);
}

void Mesh::create_ebo()
{
	glCreateBuffers(1, &ebo_);
	glNamedBufferStorage(ebo_, ebo_values_.size() * sizeof(ebo_values_[0]), ebo_values_.data(), GL_DYNAMIC_STORAGE_BIT);
}

void Mesh::create_vbo()
{
	glCreateBuffers(1, &vbo_);
	glNamedBufferStorage(vbo_, vertices_.vertices_.size() * sizeof(Vertex), vertices_.vertices_.data(), GL_DYNAMIC_STORAGE_BIT);
}

void Mesh::create_vao()
{
	const GLuint vbo_binding_index = 0;

	glCreateVertexArrays(1, &vao_);
	glBindVertexArray(vao_);
	glVertexArrayVertexBuffer(vao_, vbo_binding_index, vbo_, 0, sizeof(Vertex));
	glVertexArrayElementBuffer(vao_, ebo_);

	if(vertices_.has_positions_)
	{
		load_vertex_attribute(vbo_binding_index, AttributeIndex::Position, 3);
	}

	if(vertices_.has_normals_)
	{
		load_vertex_attribute(vbo_binding_index, AttributeIndex::Normal, 3);
	}

	if(vertices_.has_texture_coordinates_)
	{
		load_vertex_attribute(vbo_binding_index, AttributeIndex::TextureCoord, 2);
	}

	if(vertices_.has_colors_)
	{
		load_vertex_attribute(vbo_binding_index, AttributeIndex::Color, 3);
	}
}

void Mesh::load_mesh()
{
	create_ebo();
	create_vbo();
	create_vao();
}

void Mesh::draw(ShaderProgram& shader_progam)
{
	glBindVertexArray(vao_);
	glDrawElements(draw_mode_, GLsizei(ebo_values_.size()), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0); //= unbind 
}