#include "mesh.h"
#include "Logging/logging.h"
#include "gl_resource_manager.h"

#include <stb/stb_image.h>
#include <iostream>

Mesh::Mesh(const MeshId& mesh_id, const MeshInfo& mesh_info)
	: mesh_id_(mesh_id), mesh_info_(mesh_info), ebo_(0), vbo_(0), vao_(0)
{
	load_mesh();
}

Mesh::Mesh(Mesh&& mesh)
	: mesh_id_(mesh.mesh_id_), mesh_info_(mesh.mesh_info_), ebo_(mesh.ebo_), vbo_(mesh.vbo_), vao_(mesh.vao_)
{
	mesh.ebo_ = 0; //glDeleteBuffers silently ignores 0's and names that do not correspond to existing buffer objects.
	mesh.vbo_ = 0; //idem
	mesh.vao_ = 0; //idem
}

Mesh& Mesh::operator=(Mesh&& mesh)
{
	if(this == &mesh)
	{
		return *this;
	}

	destroy_all_buffers();

	mesh_id_ = mesh.mesh_id_;
	mesh_info_ = mesh.mesh_info_;
	ebo_ = mesh.ebo_;
	vbo_ = mesh.vbo_;
	vao_ = mesh.vao_;

	mesh.ebo_ = 0; //glDeleteBuffers silently ignores 0's and names that do not correspond to existing buffer objects.
	mesh.vbo_ = 0; //idem
	mesh.vao_ = 0; //idem
	return *this;
}

Mesh::~Mesh()
{
	destroy_all_buffers();
}

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
	glNamedBufferStorage(ebo_, mesh_info_.ebo_values_.size() * sizeof(mesh_info_.ebo_values_[0]), mesh_info_.ebo_values_.data(), GL_DYNAMIC_STORAGE_BIT); //TODO : voir pour le dernier argument
}

void Mesh::create_vbo()
{
	glCreateBuffers(1, &vbo_);
	glNamedBufferStorage(vbo_, mesh_info_.vertices_.get_vertices_number() * sizeof(Vertex), mesh_info_.vertices_.get_vertices_data(), GL_DYNAMIC_STORAGE_BIT); //TODO : voir pour le dernier argument
}

void Mesh::create_vao()
{
	const GLuint vbo_binding_index = 0;

	glCreateVertexArrays(1, &vao_);
	glBindVertexArray(vao_);
	glVertexArrayVertexBuffer(vao_, vbo_binding_index, vbo_, 0, GLsizei(sizeof(Vertex)));
	glVertexArrayElementBuffer(vao_, ebo_);

	if(mesh_info_.vertices_.has_position_attribute())
	{
		load_vertex_attribute(vbo_binding_index, attribute::Name::POSITION);
	}

	if(mesh_info_.vertices_.has_normal_attribute())
	{
		load_vertex_attribute(vbo_binding_index, attribute::Name::NORMAL);
	}

	if(mesh_info_.vertices_.has_texcoord_attribute())
	{
		load_vertex_attribute(vbo_binding_index, attribute::Name::TEXCOORD);
	}

	if(mesh_info_.vertices_.has_color_attribute())
	{
		load_vertex_attribute(vbo_binding_index, attribute::Name::COLOR);
	}
}

void Mesh::destroy_all_buffers() const
{
	glDeleteBuffers(1, &ebo_);
	glDeleteBuffers(1, &vbo_);
	glDeleteBuffers(1, &vao_);
}

void Mesh::create_textures()
{
	int desired_channels = 4;
	GLsizei number_of_texture_levels = 1; //TODO : utiliser une autre valeur ?

	for(std::string& texture_key : mesh_info_.texture_keys_)
	{
		Texture* t = resource::get_texture(texture_key);
		glCreateTextures(GL_TEXTURE_2D, 1, &t->texture_id_);
		glBindTextureUnit(t->texture_unit_, t->texture_id_);
		glTextureParameteri(t->texture_id_, GL_TEXTURE_WRAP_S, t->wrap_s_);
		glTextureParameteri(t->texture_id_, GL_TEXTURE_WRAP_T, t->wrap_t_);
		glTextureParameteri(t->texture_id_, GL_TEXTURE_MAG_FILTER, t->mag_filter_);
		glTextureParameteri(t->texture_id_, GL_TEXTURE_MIN_FILTER, t->min_filter_);

		int width, height, channels;
		unsigned char* pixels;
		if(!t->image_path_.empty())
		{
			if((pixels = stbi_load(t->image_path_.c_str(), &width, &height, &channels, desired_channels)) == nullptr) //4 pour que ça crashe pas pour une image RGB uniquement (ex : .jpg)
			{
				logging::log("stbi_load() returned nullptr", logging::Severity::CRITICAL);
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			if((pixels = stbi_load_from_memory(t->image_data_.data(), int(t->image_data_.size()), &width, &height, &channels, desired_channels)) == nullptr) //4 pour que ça crashe pas pour une image RGB uniquement (ex : .jpg)
			{
				logging::log("stbi_load_from_memory() returned nullptr", logging::Severity::CRITICAL);
				exit(EXIT_FAILURE);
			}
		}
		glGenerateTextureMipmap(t->texture_id_);
		glTextureStorage2D(t->texture_id_, number_of_texture_levels, GL_RGBA8, width, height);
		glTextureSubImage2D(t->texture_id_, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
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

void Mesh::draw() const
{
	glBindVertexArray(vao_);
	glDrawElements(mesh_info_.draw_mode_, GLsizei(mesh_info_.ebo_values_.size()), GL_UNSIGNED_SHORT, 0);
	//glBindVertexArray(0); //= unbind, commenté car provoque des erreurs
}