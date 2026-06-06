#include "mesh.h"

Mesh::Mesh(std::vector<GLushort> ebo_values, std::vector<Vertex> attribute_values, GLenum draw_mode)
	: ebo_values_(ebo_values), attribute_values_(attribute_values), draw_mode_(draw_mode == -1 ? GL_TRIANGLES : draw_mode)
{
	load_mesh();
}

void Mesh::create_ebo()
{
	glCreateBuffers(1, &ebo_);
	glNamedBufferStorage(ebo_, ebo_values_.size() * sizeof(ebo_values_[0]), ebo_values_.data(), GL_DYNAMIC_STORAGE_BIT);
}

void Mesh::create_vbo()
{
	glCreateBuffers(1, &vbo_);
	glNamedBufferStorage(vbo_, attribute_values_.size() * sizeof(Vertex), attribute_values_.data(), GL_DYNAMIC_STORAGE_BIT);
}

void Mesh::create_vao()
{
	glCreateVertexArrays(1, &vao_);
	glBindVertexArray(vao_);
	glVertexArrayVertexBuffer(vao_, 0, vbo_, 0, sizeof(Vertex));
	glVertexArrayElementBuffer(vao_, ebo_);
	glEnableVertexArrayAttrib(vao_, 0);
	glVertexArrayAttribFormat(vao_, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao_, 0, 0);
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
	glDrawElements(draw_mode_, ebo_values_.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0); //= unbind 
}