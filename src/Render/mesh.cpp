#include "mesh.h"

#include <stb/stb_image.h>
#include <iostream>

//TODO : créer un destructeur qui libère le VAO/VBO/EBO et les textures

Mesh::Mesh(std::vector<GLushort> ebo_values, Vertices vertices, std::vector<Texture> textures, GLenum draw_mode)
	: ebo_values_(ebo_values), vertices_(vertices), textures_(textures), ebo_(0), vbo_(0), vao_(0), draw_mode_(draw_mode == -1 ? GL_TRIANGLES : draw_mode)
{
	load_mesh();
}

Mesh::Mesh(std::vector<GLushort> ebo_values, Vertices vertices, GLenum draw_mode)
	: Mesh(ebo_values, vertices, {}, draw_mode)
{}

void Mesh::load_vertex_attribute(GLuint vbo_binding_index, attribute::Name attribute_name)
{
	attribute::Info attribute_info = Vertex::get_attribute_info(attribute_name);
	glEnableVertexArrayAttrib(vao_, attribute_info.index_);
	glVertexArrayAttribFormat(vao_, attribute_info.index_, attribute_info.component_count_, GL_FLOAT, attribute_info.normalized_, GLuint(attribute_info.offset_));
	glVertexArrayAttribBinding(vao_, attribute_info.index_, vbo_binding_index);
}

void Mesh::create_ebo()
{
	glCreateBuffers(1, &ebo_);
	glNamedBufferStorage(ebo_, ebo_values_.size() * sizeof(ebo_values_[0]), ebo_values_.data(), GL_DYNAMIC_STORAGE_BIT); //TODO : voir pour le dernier argument
}

void Mesh::create_vbo()
{
	glCreateBuffers(1, &vbo_);
	glNamedBufferStorage(vbo_, vertices_.get_vertices_number() * sizeof(Vertex), vertices_.get_vertices_data(), GL_DYNAMIC_STORAGE_BIT); //TODO : voir pour le dernier argument
}

void Mesh::create_vao()
{
	const GLuint vbo_binding_index = 0;

	glCreateVertexArrays(1, &vao_);
	glBindVertexArray(vao_);
	glVertexArrayVertexBuffer(vao_, vbo_binding_index, vbo_, 0, GLsizei(sizeof(Vertex)));
	glVertexArrayElementBuffer(vao_, ebo_);

	if(vertices_.has_position_attribute())
	{
		load_vertex_attribute(vbo_binding_index, attribute::Name::POSITION);
	}

	if(vertices_.has_normal_attribute())
	{
		load_vertex_attribute(vbo_binding_index, attribute::Name::NORMAL);
	}

	if(vertices_.has_texcoord_attribute())
	{
		load_vertex_attribute(vbo_binding_index, attribute::Name::TEXCOORD);
	}

	if(vertices_.has_color_attribute())
	{
		load_vertex_attribute(vbo_binding_index, attribute::Name::COLOR);
	}
}

void Mesh::create_textures()
{
	int desired_channels = 4;
	GLsizei number_of_texture_levels = 1; //TODO : utiliser une autre valeur ?

	for(Texture& t : textures_)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &t.texture_id_);
		glBindTextureUnit(t.texture_unit_, t.texture_id_);
		glTextureParameteri(t.texture_id_, GL_TEXTURE_WRAP_S, t.wrap_s_);
		glTextureParameteri(t.texture_id_, GL_TEXTURE_WRAP_T, t.wrap_t_);
		glTextureParameteri(t.texture_id_, GL_TEXTURE_MAG_FILTER, t.mag_filter_);
		glTextureParameteri(t.texture_id_, GL_TEXTURE_MIN_FILTER, t.min_filter_);

		int width, height, channels;
		unsigned char* pixels;
		if(!t.image_path_.empty())
		{
			if((pixels = stbi_load(t.image_path_.c_str(), &width, &height, &channels, desired_channels)) == nullptr) //4 pour que ça crashe pas pour une image RGB uniquement (ex : .jpg)
			{
				std::cerr << "Error (stbi_load)\n";
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			if((pixels = stbi_load_from_memory(t.image_data_.data(), int(t.image_data_.size()), &width, &height, &channels, desired_channels)) == nullptr) //4 pour que ça crashe pas pour une image RGB uniquement (ex : .jpg)
			{
				std::cerr << "Error (stbi_load)\n";
				exit(EXIT_FAILURE);
			}
		}
		glGenerateTextureMipmap(t.texture_id_);
		glTextureStorage2D(t.texture_id_, number_of_texture_levels, GL_RGBA8, width, height);
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

void Mesh::draw(ShaderProgram& shader_program) //TODO : paramètre inutile 
{
	glBindVertexArray(vao_);
	glDrawElements(draw_mode_, GLsizei(ebo_values_.size()), GL_UNSIGNED_SHORT, 0);
	//glBindVertexArray(0); //= unbind, commenté car provoque des erreurs
}