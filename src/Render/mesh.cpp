#include "mesh.h"
#include "Logging/logging.h"
#include "Resource/resource_manager.h"
#include "glTF/gltf.h"
#include "Common/utils.h"

#include <stb/stb_image.h>
#include <filesystem>
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

void Mesh::load_mesh()
{
	create_ebo();
	create_vbo();
	create_vao();
	create_textures();
}

void Mesh::load_vertex_attribute(GLuint vbo_binding_index, attribute::Name attribute_name)
{
	attribute::Info attribute_info = Vertex::get_attribute_info(attribute_name);

	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "Entering VAO's attribute code.");
	glEnableVertexArrayAttrib(vao_, attribute_info.index_);
	glVertexArrayAttribFormat(vao_, attribute_info.index_, attribute_info.component_count_, GL_FLOAT, attribute_info.normalized_, GLuint(attribute_info.offset_));
	glVertexArrayAttribBinding(vao_, attribute_info.index_, vbo_binding_index);
	glPopDebugGroup();
}

//TODO : inutile ?
unsigned char* Mesh::load_image(const ImageTexture& image_texture, int& width, int& height, int& channels) const
{
	unsigned char* pixels;
	if(std::holds_alternative<ImagePath>(image_texture.image_value_))
	{
		const ImagePath& image_path = std::get<ImagePath>(image_texture.image_value_);
		if((pixels = stbi_load(image_path.c_str(), &width, &height, &channels, 0)) == nullptr)
		{
			logging::log("stbi_load() returned nullptr: " + std::string(stbi_failure_reason()), logging::Severity::CRITICAL);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		const ImageData& image_data = std::get<ImageData>(image_texture.image_value_);
		if((pixels = stbi_load_from_memory(image_data.data(), int(image_data.size()), &width, &height, &channels, 0)) == nullptr)
		{
			logging::log("stbi_load_from_memory() returned nullptr: " + std::string(stbi_failure_reason()), logging::Severity::CRITICAL);
			exit(EXIT_FAILURE);
		}
	}
	return pixels;
}

void Mesh::create_ebo()
{
	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "Entering EBO creation and storage code.");
	glCreateBuffers(1, &ebo_);
	glNamedBufferStorage(ebo_, mesh_info_.ebo_values_.size() * sizeof(GLushort), mesh_info_.ebo_values_.data(), GL_DYNAMIC_STORAGE_BIT); //TODO : voir pour le dernier argument
	glPopDebugGroup();
}

void Mesh::create_vbo()
{
	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "Entering VBO creation and storage code.");
	glCreateBuffers(1, &vbo_);
	glNamedBufferStorage(vbo_, mesh_info_.vertices_.get_vertices_number() * sizeof(Vertex), mesh_info_.vertices_.get_vertices_data(), GL_DYNAMIC_STORAGE_BIT); //TODO : voir pour le dernier argument
	glPopDebugGroup();
}

void Mesh::create_vao()
{
	const GLuint vbo_binding_index = 0;

	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "Entering VAO creation, binding and content code.");
	glCreateVertexArrays(1, &vao_);
	glBindVertexArray(vao_);
	glVertexArrayVertexBuffer(vao_, vbo_binding_index, vbo_, 0, GLsizei(sizeof(Vertex)));
	glVertexArrayElementBuffer(vao_, ebo_);
	glPopDebugGroup();

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
	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "Entering EBO, VBO and VAO deletion code.");
	glDeleteBuffers(1, &ebo_);
	glDeleteBuffers(1, &vbo_);
	glDeleteVertexArrays(1, &vao_); //Attention : si j'utilise glDeleteBuffers() pour le vao, je vais avoir des problèmes !
	glPopDebugGroup();

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
	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "Entering texture creation and binding code.");
	glCreateTextures(texture->info_.target_, 1, &texture->info_.id_);
	glBindTextureUnit(texture->info_.texture_unit_, texture->info_.id_);
	glPopDebugGroup();
}

void Mesh::set_texture_storage(Texture* texture, int width, int height) const
{
	GLenum internal_format = GL_RGBA8;
	if(std::holds_alternative<ImageTexture>(texture->texture_))
	{
		const ImageTexture& image_texture = std::get<ImageTexture>(texture->texture_);
		internal_format = gltf::get_sized_enum_from_channels(image_texture.channels_);
	}
	
	GLsizei number_of_texture_levels = texture->should_have_mipmaps_ ? 1 + int(std::floor(std::log2(std::max(width, height)))) : 1;
	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "Entering texture storage code.");
	glTextureStorage2D(texture->info_.id_, number_of_texture_levels, internal_format, width, height); //ici (cas .png), il faut mettre GL_RGBA8 et non GL_RGBA car il faut mettre un "Sized Internal Format" (voir https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexStorage2D.xhtml)
	glPopDebugGroup();
}

void Mesh::set_texture_parameters(const Texture* texture) const
{
	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "Entering texture parameters code.");
	glTextureParameteri(texture->info_.id_, GL_TEXTURE_WRAP_S, texture->info_.wrap_s_);
	glTextureParameteri(texture->info_.id_, GL_TEXTURE_WRAP_T, texture->info_.wrap_t_);
	if(texture->info_.target_ == GL_TEXTURE_CUBE_MAP) //TODO : ne pas oublier de modifier pour d'autres types de textures 3D
	{
		glTextureParameteri(texture->info_.id_, GL_TEXTURE_WRAP_R, texture->info_.wrap_r_);
	}
	glTextureParameteri(texture->info_.id_, GL_TEXTURE_MAG_FILTER, texture->info_.mag_filter_);
	glTextureParameteri(texture->info_.id_, GL_TEXTURE_MIN_FILTER, texture->info_.min_filter_);
	glPopDebugGroup();
}

void Mesh::set_texture_content(Texture* texture, int width, int height, void* pixels) const //TODO : renommer set_2d_texture_content
{
	GLenum format = GL_RGBA;
	if(std::holds_alternative<ImageTexture>(texture->texture_))
	{
		const ImageTexture& image_texture = std::get<ImageTexture>(texture->texture_);
		format = gltf::get_base_enum_from_channels(image_texture.channels_);
	}

	GLint texture_level = 0; //on ne spécifie que la texture de niveau 0 et on génère automatiquement les mipmaps avec glGenerateTextureMipmap()
	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "Entering texture content code.");
	glTextureSubImage2D(texture->info_.id_, texture_level, 0, 0, width, height, format, GL_UNSIGNED_BYTE, pixels); //ici (cas .png), il faut mettre GL_RGBA et non GL_RGBA8, ce dernier n'étant pas supporté pour ce paramètre
	glGenerateTextureMipmap(texture->info_.id_);
	glPopDebugGroup();
}

void Mesh::set_cubemap_content(Texture* texture) const
{
	CubeMapTexture& cubemap_texture = std::get<CubeMapTexture>(texture->texture_);
	int width = cubemap_texture.back_.initial_width_; //Comme toutes les images de la cubemap ont la même width / height / nombre de channels, on peut récupérer les infos de n'importe laquelle (ici back)
	int height = cubemap_texture.back_.initial_height_;
	int channels = cubemap_texture.back_.channels_;
	GLenum format = gltf::get_base_enum_from_channels(channels);
	GLint texture_level = 0; //on ne spécifie que la texture de niveau 0 et on génère automatiquement les mipmaps avec glGenerateTextureMipmap()

	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "Entering cubemap content code.");
	glTextureSubImage3D(texture->info_.id_, texture_level, 0, 0, 5, width, height, 1, format, GL_UNSIGNED_BYTE, cubemap_texture.back_.pixels_); //TODO : attention, la valeur de zoffset est 5 et non 4
	glTextureSubImage3D(texture->info_.id_, texture_level, 0, 0, 3, width, height, 1, format, GL_UNSIGNED_BYTE, cubemap_texture.bottom_.pixels_);
	glTextureSubImage3D(texture->info_.id_, texture_level, 0, 0, 4, width, height, 1, format, GL_UNSIGNED_BYTE, cubemap_texture.front_.pixels_); //TODO : attention, la valeur est 4 et non 5
	glTextureSubImage3D(texture->info_.id_, texture_level, 0, 0, 1, width, height, 1, format, GL_UNSIGNED_BYTE, cubemap_texture.left_.pixels_);
	glTextureSubImage3D(texture->info_.id_, texture_level, 0, 0, 0, width, height, 1, format, GL_UNSIGNED_BYTE, cubemap_texture.right_.pixels_);
	glTextureSubImage3D(texture->info_.id_, texture_level, 0, 0, 2, width, height, 1, format, GL_UNSIGNED_BYTE, cubemap_texture.top_.pixels_);
	//TODO : l'idéal serait d'avoir des constantes pour le champ zoffset

	//for(const ImageTexture& image_texture : cubemap_texture.images_)
	//{
	//	const ImagePath& image_path = std::get<ImagePath>(image_texture.image_value_);

	//	//TODO : ce code fonctionne uniquement si la convention de nommage des fichiers est respectée
	//	//TODO : mettre les if dans une méthode
	//	GLint z_offset = 0;
	//	if(image_path.find("right") != std::string::npos)
	//	{
	//		z_offset = 0;
	//	}
	//	else if(image_path.find("left") != std::string::npos)
	//	{
	//		z_offset = 1;
	//	}
	//	else if(image_path.find("top") != std::string::npos)
	//	{
	//		z_offset = 2;
	//	}
	//	else if(image_path.find("bottom") != std::string::npos)
	//	{
	//		z_offset = 3;
	//	}
	//	else if(image_path.find("back") != std::string::npos)
	//	{
	//		z_offset = 5; //TODO : attention, la valeur est 5 et non 4
	//	}
	//	else if(image_path.find("front") != std::string::npos)
	//	{
	//		z_offset = 4; //TODO : attention, la valeur est 4 et non 5
	//	}
	//	else
	//	{
	//		std::cout << "****************************************\n";
	//	}

	//	GLenum format = gltf::get_base_enum_from_channels(channels);
	//	glTextureSubImage3D(texture->info_.id_, texture_level, 0, 0, z_offset, width, height, 1, format, GL_UNSIGNED_BYTE, image_texture.pixels_); //TODO : si j'utilise i, z_offset est inutile
	//}
	glGenerateTextureMipmap(texture->info_.id_);
	glPopDebugGroup();
}

void Mesh::create_textures()
{
	for(std::size_t texture_id : mesh_info_.texture_ids_)
	{
		Texture* texture = resource::get_texture(texture_id);
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

		init_texture(texture);

		//TODO : essayer de factoriser
		if(std::holds_alternative<ImageTexture>(texture->texture_))
		{
			ImageTexture& image_texture = std::get<ImageTexture>(texture->texture_);
			unsigned char* pixels = image_texture.pixels_;
			int width = image_texture.initial_width_;
			int height = image_texture.initial_height_;

			set_texture_storage(texture, width, height);
			set_texture_content(texture, width, height, pixels);
		}
		else if(std::holds_alternative<TextTexture>(texture->texture_))
		{
			const TextTexture& text_texture = std::get<TextTexture>(texture->texture_);
			unsigned char* pixels = static_cast<unsigned char*>(text_texture.pixels_);
			int width = text_texture.width_;
			int height = text_texture.height_;

			set_texture_storage(texture, width, height);
			set_texture_content(texture, width, height, pixels);
		}
		else if(std::holds_alternative<CubeMapTexture>(texture->texture_))
		{
			CubeMapTexture& cubemap_texture = std::get<CubeMapTexture>(texture->texture_);
			int width = cubemap_texture.back_.initial_width_; //Comme toutes les images de la cubemap ont la même width / height / nombre de channels, on peut récupérer les infos de n'importe laquelle (ici back)
			int height = cubemap_texture.back_.initial_height_;
			//int channels = cubemap_texture.back_.channels_;

			set_texture_storage(texture, width, height);
			set_cubemap_content(texture);
		}
	}
}

void Mesh::edit_text_texture(int new_width, int new_height, void* new_pixels) const
{
	Texture* texture = resource::get_texture(mesh_info_.texture_ids_[0]); //le mesh d'un Text ne contient qu'une seule texture
	TextTexture& texture_text = std::get<TextTexture>(texture->texture_);

	int old_width, old_height;
	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "Entering texture level parameters code.");
	glGetTextureLevelParameteriv(texture->info_.id_, 0, GL_TEXTURE_WIDTH, &old_width);
	glGetTextureLevelParameteriv(texture->info_.id_, 0, GL_TEXTURE_HEIGHT, &old_height);
	glPopDebugGroup();

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
	Texture* texture = resource::get_texture(mesh_info_.texture_ids_[0]); //le mesh d'une Image ne contient qu'une seule texture
	const ImageTexture& texture_image = std::get<ImageTexture>(texture->texture_);
	return texture_image.initial_width_;
}

int Mesh::get_initial_texture_height() const
{
	Texture* texture = resource::get_texture(mesh_info_.texture_ids_[0]); //le mesh d'une Image ne contient qu'une seule texture
	const ImageTexture& texture_image = std::get<ImageTexture>(texture->texture_);
	return texture_image.initial_height_;
}

void Mesh::render() const
{
	if(ShaderProgram* shader_program = resource::get_currently_bound_shader(); shader_program != nullptr)
	{
		for(std::size_t texture_id : mesh_info_.texture_ids_)
		{
			Texture* texture = resource::get_texture(texture_id);
			if(texture->info_.texture_unit_ > 0)
			{
				logging::log("Only one texture by mesh for now!", logging::Severity::WARNING);
			}
			glBindTextureUnit(texture->info_.texture_unit_, texture->info_.id_);
			if(texture->info_.target_ != GL_TEXTURE_CUBE_MAP)
			{
				shader_program->set_uniform_1i("texture_sampler0_", texture->info_.texture_unit_);
			}
			else
			{
				shader_program->set_uniform_1i("cubemap_sampler0_", texture->info_.texture_unit_);
			}
		}
	}
	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "Entering binding VAO and drawing code.");
	glBindVertexArray(vao_);
	glDrawElements(mesh_info_.render_mode_, GLsizei(mesh_info_.ebo_values_.size()), GL_UNSIGNED_SHORT, 0);
	//glBindVertexArray(0); //= unbind, commenté car provoque des erreurs
	glPopDebugGroup();
}