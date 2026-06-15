#include "mesh.h"

#include <stb/stb_image.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

//TODO : créer un destructeur qui libère le VAO/VBO/EBO et les textures

Mesh::Mesh(std::vector<GLushort> ebo_values, Vertices vertices, std::vector<Texture::TextureInfo> textures_info, glm::mat4 transformation_matrix, GLenum draw_mode)
	: ebo_values_(ebo_values), vertices_(vertices), transformation_matrix_(transformation_matrix), draw_mode_(draw_mode == -1 ? GL_TRIANGLES : draw_mode)
{
	for(const Texture::TextureInfo& t_info : textures_info)
	{
		textures_.push_back({0, t_info}); //texture id = 0 initialement
	}
	load_mesh();
}

void Mesh::load_vertex_attribute(GLuint vbo_binding_index, Vertices::AttributeIndex attribute_index, GLint size)
{
	std::size_t offset = vertices_.get_attribute_offset(attribute_index);
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
	glNamedBufferStorage(vbo_, vertices_.vertices_.size() * sizeof(Vertices::Vertex), vertices_.vertices_.data(), GL_DYNAMIC_STORAGE_BIT);
}

void Mesh::create_vao()
{
	const GLuint vbo_binding_index = 0;

	glCreateVertexArrays(1, &vao_);
	glBindVertexArray(vao_);
	glVertexArrayVertexBuffer(vao_, vbo_binding_index, vbo_, 0, sizeof(Vertices::Vertex));
	glVertexArrayElementBuffer(vao_, ebo_);

	if(vertices_.has_positions_)
	{
		load_vertex_attribute(vbo_binding_index, Vertices::AttributeIndex::Position, 3);
	}

	if(vertices_.has_normals_)
	{
		load_vertex_attribute(vbo_binding_index, Vertices::AttributeIndex::Normal, 3);
	}

	if(vertices_.has_texture_coordinates_)
	{
		load_vertex_attribute(vbo_binding_index, Vertices::AttributeIndex::TextureCoord, 2);
	}

	if(vertices_.has_colors_)
	{
		load_vertex_attribute(vbo_binding_index, Vertices::AttributeIndex::Color, 3);
	}
}

void Mesh::create_textures()
{
	for(Texture& t : textures_)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &t.texture_id_);
		glBindTextureUnit(t.texture_info_.texture_index_, t.texture_id_);
		glTextureParameteri(t.texture_id_, GL_TEXTURE_WRAP_S, t.texture_info_.wrap_s_);
		glTextureParameteri(t.texture_id_, GL_TEXTURE_WRAP_T, t.texture_info_.wrap_t_);
		glTextureParameteri(t.texture_id_, GL_TEXTURE_MAG_FILTER, t.texture_info_.mag_filter_);
		glTextureParameteri(t.texture_id_, GL_TEXTURE_MIN_FILTER, t.texture_info_.min_filter_);

		int width, height, channels;
		unsigned char* pixels;
		if((pixels = stbi_load(t.texture_info_.image_path_.c_str(), &width, &height, &channels, 4)) == nullptr) //4 pour que ça crashe pas pour une image RGB uniquement (ex : .jpg)
		{
			std::cerr << "Error (stbi_load)\n";
			exit(EXIT_FAILURE);
		}

		glGenerateTextureMipmap(t.texture_id_);
		glTextureStorage2D(t.texture_id_, 1, GL_RGBA8, width, height); //TODO : 1 = number of texture levels => utiliser une autre valeur ?
		glTextureSubImage2D(t.texture_id_, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels); 
		stbi_image_free(pixels);
	}
}

void Mesh::load_mesh()
{
	create_ebo();
	create_vbo();
	create_vao();
	create_textures();
}

void Mesh::draw(ShaderProgram& shader_progam)
{
	shader_progam.set_uniform_matrix_4fv("transformation_matrix", glm::value_ptr(transformation_matrix_));
	glBindVertexArray(vao_);
	glDrawElements(draw_mode_, GLsizei(ebo_values_.size()), GL_UNSIGNED_SHORT, 0);
	//glBindVertexArray(0); //= unbind, provoque des erreurs
}