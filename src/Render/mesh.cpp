#include "mesh.h"
#include "Logging/logging.h"
#include "Resource/resource_manager.h"
#include "glTF/gltf.h"

#include <stb/stb_image.h>
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

void Mesh::init_texture(Texture* texture) const
{
	create_and_bind_texture(texture);
	set_texture_parameters(texture);
}

void Mesh::create_and_bind_texture(Texture* texture) const
{
	glCreateTextures(GL_TEXTURE_2D, 1, &texture->info_.id_);
	glBindTextureUnit(texture->info_.texture_unit_, texture->info_.id_);
}

void Mesh::set_texture_storage(Texture* texture, int width, int height) const
{
	GLenum internal_format = GL_RGBA8;
	if(std::holds_alternative<ImageTexture>(texture->texture_))
	{
		ImageTexture& image_texture = std::get<ImageTexture>(texture->texture_);
		internal_format = gltf::get_sized_enum_from_channels(image_texture.channels_);
	}
	//std::cout << "STORAGE\n";
	GLsizei number_of_texture_levels = texture->should_have_mipmaps_ ? 1 + int(std::floor(std::log2(std::max(width, height)))) : 1;
	glTextureStorage2D(texture->info_.id_, number_of_texture_levels, internal_format, width, height); //ici (cas .png), il faut mettre GL_RGBA8 et non GL_RGBA car il faut mettre un "Sized Internal Format" (voir https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexStorage2D.xhtml)
	//std::cout << "STORAGE END\n";
}

void Mesh::set_texture_parameters(const Texture* texture) const
{
	//std::cout << "PARAMETERS\n";
	glTextureParameteri(texture->info_.id_, GL_TEXTURE_WRAP_S, texture->info_.wrap_s_);
	glTextureParameteri(texture->info_.id_, GL_TEXTURE_WRAP_T, texture->info_.wrap_t_);
	glTextureParameteri(texture->info_.id_, GL_TEXTURE_MAG_FILTER, texture->info_.mag_filter_);
	glTextureParameteri(texture->info_.id_, GL_TEXTURE_MIN_FILTER, texture->info_.min_filter_);
	//std::cout << "PARAMETERS END\n";
}

void Mesh::set_texture_content(Texture* texture, int width, int height, void* pixels) const
{
	GLenum format = GL_RGBA;
	if(std::holds_alternative<ImageTexture>(texture->texture_))
	{
		ImageTexture& image_texture = std::get<ImageTexture>(texture->texture_);
		format = gltf::get_base_enum_from_channels(image_texture.channels_);
	}

	//std::cout << "CONTENT\n";
	GLint texture_level = 0; //on ne spécifie que la texture de niveau 0 et on génère automatiquement les mipmaps avec glGenerateTextureMipmap()
	glTextureSubImage2D(texture->info_.id_, texture_level, 0, 0, width, height, format, GL_UNSIGNED_BYTE, pixels); //ici (cas .png), il faut mettre GL_RGBA et non GL_RGBA8, ce dernier n'étant pas supporté pour ce paramètre
	glGenerateTextureMipmap(texture->info_.id_);
	//std::cout << "CONTENT END\n";
}

void Mesh::create_textures()
{
	for(std::string& texture_key : mesh_info_.texture_keys_)
	{
		Texture* texture = resource::get_texture(texture_key);
		if(texture == nullptr)
		{
			logging::log("The texture is nullptr!", logging::Severity::WARNING);
			continue;
		}
		else if(std::holds_alternative<TextTexture>(texture->texture_))
		{ 
			if(TextTexture& text_texture = std::get<TextTexture>(texture->texture_); text_texture.width_ == 0 && text_texture.height_ == 0 && text_texture.pixels_ == nullptr)
			{
				logging::log("The text texture is empty!", logging::Severity::DEBUG);
				init_texture(texture);
				continue;
			}
		}
		
		int width, height;
		unsigned char* pixels = nullptr;

		if(std::holds_alternative<ImageTexture>(texture->texture_))
		{
			ImageTexture& image_texture = std::get<ImageTexture>(texture->texture_);
			int channels;

			if(std::holds_alternative<ImagePath>(image_texture.image_value_))
			{
				ImagePath& image_path = std::get<ImagePath>(image_texture.image_value_);
				if((pixels = stbi_load(image_path.c_str(), &width, &height, &channels, 0)) == nullptr)
				{
					logging::log("stbi_load() returned nullptr: " + std::string(stbi_failure_reason()), logging::Severity::CRITICAL);
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				ImageData& image_data = std::get<ImageData>(image_texture.image_value_);
				if((pixels = stbi_load_from_memory(image_data.data(), int(image_data.size()), &width, &height, &channels, 0)) == nullptr)
				{
					logging::log("stbi_load_from_memory() returned nullptr: " + std::string(stbi_failure_reason()), logging::Severity::CRITICAL);
					exit(EXIT_FAILURE);
				}
			}
			image_texture.channels_ = channels;
			image_texture.initial_width_ = width;
			image_texture.initial_height_ = height;
		}
		else if(std::holds_alternative<TextTexture>(texture->texture_))
		{
			TextTexture& text_texture = std::get<TextTexture>(texture->texture_);
			pixels = static_cast<unsigned char*>(text_texture.pixels_);
			width = text_texture.width_;
			height = text_texture.height_;
		}
		else
		{
			width = 0;
			height = 0;
		}

		init_texture(texture);
		set_texture_storage(texture, width, height);
		set_texture_content(texture, width, height, pixels);

		if(std::holds_alternative<ImageTexture>(texture->texture_))
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
	TextTexture& texture_text = std::get<TextTexture>(texture->texture_);

	//std::cout << "TEXTURE LEVEL PARAM\n";
	int old_width, old_height;
	glGetTextureLevelParameteriv(texture->info_.id_, 0, GL_TEXTURE_WIDTH, &old_width);
	glGetTextureLevelParameteriv(texture->info_.id_, 0, GL_TEXTURE_HEIGHT, &old_height);
	//std::cout << "TEXTURE LEVEL PARAM END\n";

	if(new_width != old_width || new_height != old_height)
	{
		texture_text.width_ = new_width;
		texture_text.height_ = new_height;

		glDeleteTextures(1, &texture->info_.id_);
		init_texture(texture);
		set_texture_storage(texture, new_width, new_height);
	}

	texture_text.pixels_ = new_pixels;
	set_texture_content(texture, new_width, new_height, new_pixels);
}

int Mesh::get_initial_texture_width() const
{
	Texture* texture = resource::get_texture(mesh_info_.texture_keys_[0]); //le mesh d'une Image ne contient qu'une seule texture
	ImageTexture& texture_image = std::get<ImageTexture>(texture->texture_);
	return texture_image.initial_width_;
}

int Mesh::get_initial_texture_height() const
{
	Texture* texture = resource::get_texture(mesh_info_.texture_keys_[0]); //le mesh d'une Image ne contient qu'une seule texture
	ImageTexture& texture_image = std::get<ImageTexture>(texture->texture_);
	return texture_image.initial_height_;
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
			//std::cout << "BIND\n";
			glBindTextureUnit(texture->info_.texture_unit_, texture->info_.id_);
			//std::cout << "END BIND\n";
			//std::cout << "SAMPLER: " << shader_program->get_shader_program_name() << ", num: " << mesh_info_.texture_keys_.size() << std::endl;
			shader_program->set_uniform_1i("texture_sampler0_", texture->info_.texture_unit_);
		}
	}
	glBindVertexArray(vao_);
	glDrawElements(mesh_info_.render_mode_, GLsizei(mesh_info_.ebo_values_.size()), GL_UNSIGNED_SHORT, 0);
	//glBindVertexArray(0); //= unbind, commenté car provoque des erreurs
}