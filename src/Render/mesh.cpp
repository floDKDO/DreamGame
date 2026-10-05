#include "mesh.h"
#include "Logging/logging.h"
#include "Resource/gl_resource_manager.h"
#include "RAII_SDL3/font.h"
#include "RAII_SDL3/surface.h"

#include <stb/stb_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>

Mesh::Mesh(const Id& mesh_id, const Info& mesh_info)
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

void Mesh::destroy_all_buffers()
{
	glDeleteBuffers(1, &ebo_);
	glDeleteBuffers(1, &vbo_);
	glDeleteVertexArrays(1, &vao_); //Attention : si j'utilise glDeleteBuffers() pour le vao, je vais avoir des problèmes !

	ebo_ = 0;
	vbo_ = 0;
	vao_ = 0;
}

void Mesh::create_textures()
{
	int desired_channels = 4;
	GLsizei number_of_texture_levels = 1; //TODO : utiliser une autre valeur ?

	for(std::string& texture_key : mesh_info_.texture_keys_)
	{
		Texture* t = resource::get_texture(texture_key);
		if(t == nullptr)
		{
			logging::log("The texture is nullptr!", logging::Severity::WARNING);
			continue;
		}
		glCreateTextures(GL_TEXTURE_2D, 1, &t->info_.id_);
		glBindTextureUnit(t->info_.texture_unit_, t->info_.id_);
		glTextureParameteri(t->info_.id_, GL_TEXTURE_WRAP_S, t->info_.wrap_s_);
		glTextureParameteri(t->info_.id_, GL_TEXTURE_WRAP_T, t->info_.wrap_t_);
		glTextureParameteri(t->info_.id_, GL_TEXTURE_MAG_FILTER, t->info_.mag_filter_);
		glTextureParameteri(t->info_.id_, GL_TEXTURE_MIN_FILTER, t->info_.min_filter_);

		int width = 0, height = 0, channels = 0;
		unsigned char* pixels = nullptr;

		if(std::holds_alternative<ImageTexture>(t->texture_))
		{
			ImageTexture& image_texture = std::get<ImageTexture>(t->texture_);
			if(!image_texture.image_path_.empty())
			{
				if((pixels = stbi_load(image_texture.image_path_.c_str(), &width, &height, &channels, desired_channels)) == nullptr) //4 pour que ça crashe pas pour une image RGB uniquement (ex : .jpg)
				{
					logging::log("stbi_load() returned nullptr: " + std::string(stbi_failure_reason()), logging::Severity::CRITICAL);
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				if((pixels = stbi_load_from_memory(image_texture.image_data_.data(), int(image_texture.image_data_.size()), &width, &height, &channels, desired_channels)) == nullptr) //4 pour que ça crashe pas pour une image RGB uniquement (ex : .jpg)
				{
					logging::log("stbi_load_from_memory() returned nullptr: " + std::string(stbi_failure_reason()), logging::Severity::CRITICAL);
					exit(EXIT_FAILURE);
				}
			}
		}
		else if(std::holds_alternative<TextTexture>(t->texture_))
		{
			TextTexture& text_texture = std::get<TextTexture>(t->texture_);
			pixels = static_cast<unsigned char*>(text_texture.pixels_);
			width = text_texture.width_;
			height = text_texture.height_;
		}

		glTextureStorage2D(t->info_.id_, number_of_texture_levels, GL_RGBA8, width, height);
		glGenerateTextureMipmap(t->info_.id_);
		glTextureSubImage2D(t->info_.id_, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		if(std::holds_alternative<ImageTexture>(t->texture_))
		{
			stbi_image_free(pixels);
		}
	}
}

void Mesh::load_mesh()
{
	create_ebo();
	create_vbo();
	create_vao();
	create_textures();
}

void Mesh::edit_text_texture(int new_width, int new_height, void* new_pixels) const
{
	Texture* texture = resource::get_texture(mesh_info_.texture_keys_[0]); //le mesh d'un Text ne contient qu'une seule texture
	int old_width, old_height;
	glGetTextureLevelParameteriv(texture->info_.id_, 0, GL_TEXTURE_WIDTH, &old_width);
	glGetTextureLevelParameteriv(texture->info_.id_, 0, GL_TEXTURE_HEIGHT, &old_height);
	if(new_width != old_width || new_height != old_height)
	{
		glDeleteTextures(1, &texture->info_.id_);
		glCreateTextures(GL_TEXTURE_2D, 1, &texture->info_.id_);
		glTextureStorage2D(texture->info_.id_, 1, GL_RGBA8, new_width, new_height);
	}
	glTextureSubImage2D(texture->info_.id_, 0, 0, 0, new_width, new_height, GL_RGBA, GL_UNSIGNED_BYTE, new_pixels);
}

void Mesh::render() const
{
	if(ShaderProgram* shader_program = resource::get_currently_bound_shader(); shader_program != nullptr)
	{
		for(const std::string& texture_key : mesh_info_.texture_keys_)
		{
			Texture* texture = resource::get_texture(texture_key);
			if(texture->info_.texture_unit_ > 0)
			{
				logging::log("Only one texture by mesh for now!", logging::Severity::WARNING);
			}
			glBindTextureUnit(texture->info_.texture_unit_, texture->info_.id_);
			shader_program->set_uniform_1i("texture_sampler0_", texture->info_.texture_unit_);
		}
	}
	glBindVertexArray(vao_);
	glDrawElements(mesh_info_.render_mode_, GLsizei(mesh_info_.ebo_values_.size()), GL_UNSIGNED_SHORT, 0);
	//glBindVertexArray(0); //= unbind, commenté car provoque des erreurs
}